
#include "minishell.h"


/* ---- local helpers ----------------------------------------------------- */

int is_quoted(const char *s)
{
    if (!s) return 0;
    size_t len = ft_strlen(s);
    return (len >= 2 && ((s[0] == '\'' && s[len-1] == '\'') ||
                         (s[0] == '"'  && s[len-1] == '"')));
}

char *strip_outer_quotes(const char *s)
{
    if (!s) return NULL;
    size_t len = ft_strlen(s);
    if (len >= 2 && ((s[0] == '\'' && s[len-1] == '\'') ||
                     (s[0] == '"'  && s[len-1] == '"')))
        return ft_substr(s, 1, len - 2);
    return ft_strdup(s);
}

static int is_delim_line(const char *line, const char *delim)
{
    if (!line || !delim) return 0;
    return (ft_strcmp(line, delim) == 0);
}

/* ---- public helpers ---------------------------------------------------- */

/* normalize the delimiter: strip quotes and set expand flag */
void heredoc_normalize_delimiter(t_redir *redir)
{
    if (!redir || !redir->target) return;

    // Use quoted field from redir struct
    redir->hd_expand = !redir->quoted;

    char *clean = strip_outer_quotes(redir->target);
    if (clean)
    {
        free(redir->target);
        redir->target = clean;
    }
}

/* prepare heredoc: create a pipe, read user input until delimiter */
// int heredoc_prepare(t_redir *redir, char **envp, int last_exit)
int	heredoc_prepare(t_redir *redir, t_program *program)
{
	int		pipefd[2];
	char	*line;
	pid_t	pid;
	int		status;
	int		current_line;

	current_line = 1;
	if (!redir || pipe(pipefd) == -1)
	{
		perror("heredoc: pipe failed");
		return (1);
	}
	// Fork a child process for the heredoc
	pid = fork();
	if (pid < 0)
	{
		perror("heredoc: fork failed");
		close_fd(&pipefd[0]);
		close_fd(&pipefd[1]);
		return (1);
	}
	if (pid == 0)
	{
	// Child process
		close_fd(&pipefd[0]); // Close read end in child
		set_signal_heredoc(); // Set signal handlers for heredoc
		g_signal_value = 0; // Reset signal value
		heredoc_normalize_delimiter(redir);//new position
		while (1) // Read lines until delimiter or signal
		{
			line = readline("> ");
			if (!line)//needed EOF detected
			{
				if (g_signal_value == SIGINT)//needed
				{
					close_fd(&pipefd[1]);
					exit(130);//ctr+c
				}
				fprintf(stderr, "warning: here-document at line %d delimited by end-of-file (wanted `%s')\n",
						current_line,
						redir->target); //warning in ctr+D
				break;
			}
			if (is_delim_line(line, redir->target))	// Check for delimiter
			{
				free(line);
				break ;
			}
			// Process the line
			char *to_write;
			if (redir->hd_expand)
			{
			// DEBUG removed
				to_write = expand_token_text(line, &program->envp_cpy, program->last_exit_status);
			}
			else
			{
				// DEBUG removed
				to_write = ft_strdup(line);
			}
			free(line);
			if (!to_write)
			{
				close_fd(&pipefd[1]);
				exit(1);
			}
			// Write to pipe
			write(pipefd[1], to_write, ft_strlen(to_write));
			write(pipefd[1], "\n", 1);  // Preserve newline
			free(to_write);
			current_line++;//new for warning message
		}
		// Clean up and exit
		close_fd(&pipefd[1]);
		exit(0);
	}
	// Parent process
	close_fd(&pipefd[1]);  // Close write end in parent
	redir->fd = pipefd[0];  // Store read end for command
	waitpid(pid, &status, 0);// Wait for child to finish
	// If child was interrupted by Ctrl+C (status 130)
	if (WIFEXITED(status) && WEXITSTATUS(status) == 130)
	{
		ft_printf(stderr, "inside the loop ctr c\n");//TEST
		g_signal_value = SIGINT;
		close_fd(&pipefd[0]);
		set_signal_prompt();
		if (program->line)//new
		{
			free(program->line);//new
			program->line = NULL;//new
		}
		rl_replace_line("", 0); // borra la línea actual en readline
		rl_on_new_line(); // mueve readline a una nueva línea
		rl_redisplay(); 
		return (1);
	}
    if (WIFSIGNALED(status) && WTERMSIG(status) == SIGINT)//NEW
    {
        // El hijo terminó por SIGINT (ej: Ctrl+C en heredoc)
        g_signal_value = SIGINT;
        close_fd(&pipefd);
        if (program->line)
        {
            free(program->line);
            program->line = NULL;
        }
        rl_replace_line("", 0);
        rl_on_new_line();
        rl_redisplay();
        set_signal_prompt(); // Nuevo: restaurar handler del prompt
        return 1;
    }
	// Handle other errors
	if (WIFEXITED(status) && WEXITSTATUS(status) != 0)
	{
		fprintf(stderr, "inside the other errors if\n");//test
		close_fd(&pipefd[0]);
		set_signal_prompt();//restore handler
		return 1;
	}
	// Success
	tcsetattr(STDIN_FILENO, TCSANOW, &program->orig_termios);//new -restore terminal signals ONLY IN PARENT
	set_signal_prompt();
	return (0);
}
