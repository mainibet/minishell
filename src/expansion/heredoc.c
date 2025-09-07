
#include "minishell.h"


/* ---- local helpers ----------------------------------------------------- */

int	is_quoted(const char *s)
{
	size_t	len;

	if (!s) 
		return (0);
	len = ft_strlen(s);
	return (len >= 2 && ((s[0] == '\'' && s[len - 1] == '\'')
			|| (s[0] == '"' && s[len - 1] == '"')));
}

char	*strip_outer_quotes(const char *s)
{
	size_t	len;

	if (!s)
		return (NULL);
	len = ft_strlen(s);
	if (len >= 2 && ((s[0] == '\'' && s[len - 1] == '\'') 
			|| (s[0] == '"' && s[len - 1] == '"')))
		return (ft_substr(s, 1, len - 2));
	return (ft_strdup(s));
}

static int	is_delim_line(const char *line, const char *delim)
{
	if (!line || !delim)
		return (0);
	return (ft_strcmp(line, delim) == 0);
}

/* ---- public helpers ---------------------------------------------------- */

/* normalize the delimiter: strip quotes and set expand flag */
// Use quoted field from redir struct
void	heredoc_normalize_delimiter(t_redir *redir)
{
	char	*clean;

	if (!redir || !redir->target)
		return ;
	redir->hd_expand = !redir->quoted;
	clean = strip_outer_quotes(redir->target);
	if (clean)
	{
		free(redir->target);
		redir->target = clean;
	}
}

// /* prepare heredoc: create a pipe, read user input until delimiter */
// // int heredoc_prepare(t_redir *redir, char **envp, int last_exit)
// int	heredoc_prepare(t_redir *redir, t_program *program)
// {
// 	int		pipefd[2];
// 	char	*line;
// 	pid_t	pid;
// 	int		status;
// 	int		current_line;
// 	char	*buf; //new

// 	buf = NULL;
// 	current_line = 1;
// 	if (!redir || pipe(pipefd) == -1)
// 	{
// 		perror("heredoc: pipe failed");
// 		return (1);
// 	}
// 	// Fork a child process for the heredoc
// 	pid = fork();
// 	if (pid < 0)
// 	{
// 		perror("heredoc: fork failed");
// 		close_fd(&pipefd[0]);
// 		close_fd(&pipefd[1]);
// 		return (1);
// 	}
// 	if (pid == 0)
// 	{
// 	// Child process
// 		close_fd(&pipefd[0]); // Close read end in child
// 		set_signal_heredoc(); // Set signal handlers for heredoc
// 		g_signal_value = 0; // Reset signal value
// 		heredoc_normalize_delimiter(redir);//new position
// 		while (1) // Read lines until delimiter or signal
// 		{
// 			line = readline("> ");
// 			if (!line)//needed EOF detected
// 			{
// 				if (g_signal_value == SIGINT)//needed
// 				{
// 					// free (line);
// 					// free (buf);
// 					close_fd(&pipefd[1]);
// 					exit(130);//ctr+c
// 				}
// 				if (buf) // si ya escribimos algo en buf, mandarlo al pipe
// 				 {
// 					char *to_write = buf;
// 					if (redir->hd_expand) // expandir si corresponde
// 					{
// 						to_write = expand_token_text(buf, program->envp_cpy, program->last_exit_status);
// 						// free(buf);
// 						free (line);
// 					}
// 					write(pipefd[1], to_write, ft_strlen(to_write));
// 					// free(to_write);
// 					if (redir->hd_expand)
// 						free(to_write);
// 					else
// 						free(buf);
// 					line = NULL;//new
// 					buf = NULL;
// 				}
// 				fprintf(stderr, "warning: here-document at line %d delimited by end-of-file (wanted `%s')\n",
// 						current_line,
// 						redir->target); //warning in ctr+D
// 				// free(buf);not! makes double free
// 				break;
// 			}
// 			if (is_delim_line(line, redir->target))	// Check for delimiter
// 			{
// 				free(line);
// 				break ;
// 			}
// 			//append line
// 			char *tmp = buf;//concatenate buf
// 			buf = buf ? ft_strjoin(buf, line) : ft_strdup(line);//check handle malloc and free
// 			free(tmp);
// 			// free(to_write);
// 			//add new line
// 			tmp = buf;
// 			buf = ft_strjoin(buf, "\n");
// 			free (tmp);
// 			free (line);
// 			current_line++;//new for warning message
// 		}
// 		// Process the line and expand if needed
// 		if (redir->hd_expand && buf)
// 		{
// 			char *expanded = expand_token_text(buf, program->envp_cpy, program->last_exit_status);
// 			free(buf);
// 			buf = expanded;
// 		}		// Write in pipe, clean up and exit
// 		if (buf)
// 		{
// 			write(pipefd[1], buf, ft_strlen(buf));
// 			free(buf);
// 		}
// 		close_fd(&pipefd[1]);
// 		exit(0);
// 	}
// 	// Parent process
// 	close_fd(&pipefd[1]);  // Close write end in parent
// 	redir->fd = pipefd[0];  // Store read end for command
// 	set_signal_prompt(1);//new
// 	waitpid(pid, &status, 0);// Wait for child to finish
// 	set_signal_prompt(0);//new
// 	// If child was interrupted by Ctrl+C (status 130)
// 	if ((WIFEXITED(status) && WEXITSTATUS(status) == 130)
// 		|| (WIFSIGNALED(status) && WTERMSIG(status) == SIGINT))
// 	{
// 		g_signal_value = SIGINT;
// 		close_fd(&pipefd[0]);
// 		if (program->line)//new
// 		{
// 			free(program->line);//new
// 			program->line = NULL;//new
// 		}
// 		rl_replace_line("", 0); // borra la línea actual en readline
// 		rl_on_new_line(); // mueve readline a una nueva línea
// 		rl_redisplay(); 
// 		return (1);
// 	}
// 	// Handle other errors
// 	if (WIFEXITED(status) && WEXITSTATUS(status) != 0)
// 	{
// 		close_fd(&pipefd[0]);
// 		return (1);
// 	}
// 	// Success
// 	tcsetattr(STDIN_FILENO, TCSANOW, &program->orig_termios);//new -restore terminal signals ONLY IN PARENT
// 	set_signal_prompt(0);
// 	return (0);
// }

void	write_tmp_heredoc(t_redir *redir, t_program *program,
	char **buf, char **line, int *pipefd)//fix param #
{
	char	*to_write;

	to_write = *buf;
	if (redir->hd_expand)
	{
		to_write = expand_token_text(*buf, program->envp_cpy,
				program->last_exit_status);
		if (line)
		{
			free (*line);
			*line = NULL;
		}
	}
	write(*pipefd, to_write, ft_strlen(to_write));
	if (redir->hd_expand)
		free(to_write);
	else
		free(*buf);
	*line = NULL;
	*buf = NULL;
}


//concatenate buf in tmp later maybe in fd
void	process_line_heredoc(t_redir *redir, t_program *program,
		char **buf, char *line)
{
	char	*tmp;

	tmp = *buf;
	if (*buf)
	{
		tmp = *buf;
		*buf = ft_strjoin(*buf, line);
	}
	else
		*buf = ft_strdup(line);
	tmp = *buf;
	*buf = ft_strjoin(*buf, "\n");
	free (tmp);
	free (line);
}

// Process the line and expand if needed
void	expand_heredoc(t_program *program, char **buf)
{
	char	*expanded;

	expanded = expand_token_text(*buf, program->envp_cpy,
			program->last_exit_status);
	free(*buf);
	*buf = expanded;
}

void	exit_ctrlc_heredoc(int *pipefd, char **line, char **buf)
{
	if (line)
		free (line);
	if (buf && *buf)
		free (*buf);
	close_fd(&pipefd[1]);
	exit(130);
}

int	read_heredoc(t_program *program, t_redir *redir,
		char **buf, int *pipefd, int current_line)//fix # param
{
	char	*line;

	while (1)
	{
		line = readline("> ");
		if (!line)
		{
			if (g_signal_value == SIGINT)
				exit_ctrlc_heredoc(pipefd, &line, &buf);
			if (buf && *buf)
				write_tmp_heredoc(redir, program, buf, &line, &pipefd[1]);
			fprintf(stderr,
				"warning: here-document at line %d "
				"delimited by end-of-file (wanted `%s')\n",
				current_line, redir->target);
			break ;
		}
		if (is_delim_line(line, redir->target))
		{
			free(line);
			break ;
		}
		process_line_heredoc(redir, program, buf, line);

		current_line++;
	}
	return (current_line);
}

// Close read end in child
// Set signal handlers for heredoc
// Reset signal value
// Read lines until delimiter or signal
// if (!line) is EOF detected
//check delimmiter
//process line heredoc
void	child_heredoc(t_redir *redir, t_program *program, int pipefd[2])
{
	char	*buf;
	int		current_line;

	buf = NULL;
	current_line = 1;
	close_fd(&pipefd[0]);
	set_signal_heredoc();
	g_signal_value = 0;
	heredoc_normalize_delimiter(redir);
	current_line = read_heredoc(program, redir, &buf,
			pipefd, current_line);
	if (redir->hd_expand && buf)
		expand_heredoc(program, &buf);
	if (buf)
	{
		write(pipefd[1], buf, ft_strlen(buf));
		free(buf);
	}
	close_fd(&pipefd[1]);
	exit(0);
}

//parent after ctrl c
int	clean_ctrl_c_heredoc(t_program *program, int *pipefd)
{
	g_signal_value = SIGINT;
	close_fd(&pipefd[0]);
	if (program->line)
	{
		free(program->line);
		program->line = NULL;
	}
	rl_replace_line("", 0);
	rl_on_new_line();
	rl_redisplay(); 
	return (1);
}

int	other_error_heredoc(int *pipefd)
{
	close_fd(&pipefd[0]);
	return (1);
}

// Close write end in parent
// Store read end for command
// Wait for child to finish
// If child was interrupted by Ctrl+C (status 130)
//handles other errors
//restore terminal when success and set signal promopt
int	parent_heredoc(t_redir *redir, t_program *program, int pipefd[2], int pid)
{
	int	status;

	close_fd(&pipefd[1]);
	redir->fd = pipefd[0];
	set_signal_prompt(1);
	waitpid(pid, &status, 0);
	set_signal_prompt(0);
	if ((WIFEXITED(status) && WEXITSTATUS(status) == 130)
		|| (WIFSIGNALED(status) && WTERMSIG(status) == SIGINT))
		clean_ctrl_c_heredoc(program, &pipefd[0]);
	if (WIFEXITED(status) && WEXITSTATUS(status) != 0)
		other_error_heredoc(&pipefd[0]);
	tcsetattr(STDIN_FILENO, TCSANOW, &program->orig_termios);
	set_signal_prompt(0);
	return (0);
}

/* prepare heredoc: create a pipe, read user input until delimiter */
// int heredoc_prepare(t_redir *redir, char **envp, int last_exit)
// Fork a child process for the heredoc
int	heredoc_prepare(t_redir *redir, t_program *program)
{
	int		pipefd[2];
	pid_t	pid;

	if (!redir || pipe(pipefd) == -1)
	{
		perror("heredoc: pipe failed");
		return (1);
	}
	pid = fork();
	if (pid < 0)
	{
		perror("heredoc: fork failed");
		close_fd(&pipefd[0]);
		close_fd(&pipefd[1]);
		return (1);
	}
	if (pid == 0)
		child_heredoc(redir, program, pipefd);
	return (parent_heredoc (redir, program, pipefd, pid));
}
