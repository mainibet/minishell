/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: albetanc <albetanc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/07 17:29:36 by albetanc          #+#    #+#             */
/*   Updated: 2025/09/07 17:29:37 by albetanc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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

// void	write_tmp_heredoc(t_redir *redir, t_program *program,
// 	char **buf, char **line, int *pipefd)//fix param #
void	write_tmp_heredoc(t_heredoc *hd, char **line)
{
	char	*to_write;

	to_write = hd->buf;
	if (hd->redir->hd_expand)
	{
		to_write = expand_token_text(hd->buf, hd->program->envp_cpy,
				hd->program->last_exit_status);
		if (line)
		{
			free (*line);
			*line = NULL;
		}
	}
	write(hd->pipefd[1], to_write, ft_strlen(to_write));
	if (hd->redir->hd_expand)
		free(to_write);
	else
		free(hd->buf);
	*line = NULL;
	hd->buf = NULL;
}


//concatenate buf in tmp later maybe in fd
void	process_line_heredoc(t_heredoc *hd, char *line)
{
	char	*tmp;

	tmp = hd->buf;
	if (hd->buf)
	{
		tmp = hd->buf;
		hd->buf = ft_strjoin(hd->buf, line);
	}
	else
		hd->buf = ft_strdup(line);
	tmp = hd->buf;
	hd->buf = ft_strjoin(hd->buf, "\n");
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

// void	exit_ctrlc_heredoc(int *pipefd, char **line, char **buf)
void	exit_ctrlc_heredoc(t_heredoc *hd, char **line)
{
	if (line)
		free (line);
	if (hd->buf && hd->buf)
		free (hd->buf);
	close_fd(&hd->pipefd[1]);
	exit(130);
}

char	*set_warn_ctrl_d(void)
{
	return ("warning: here-document at line %d "
		"delimited by end-of-file (wanted `%s')\n");
}

// int	read_heredoc(t_program *program, t_redir *redir,
// 		char **buf, int *pipefd, int current_line)//fix # param
int	read_heredoc(t_heredoc *hd)
{
	char	*line;

	while (1)
	{
		line = readline("> ");
		if (!line)
		{
			if (g_signal_value == SIGINT)
				exit_ctrlc_heredoc(hd, &line);
			if (hd->buf)
				write_tmp_heredoc(hd, &line);
			fprintf(stderr, set_warn_ctrl_d(), hd->current_line, hd->delimiter);
			// fprintf(stderr,
			// 	"warning: here-document at line %d "
			// 	"delimited by end-of-file (wanted `%s')\n",
				// hd->current_line, hd->redir->target);
			break ;
		}
		if (is_delim_line(line, hd->redir->target))
		{
			free(line);
			break ;
		}
		process_line_heredoc(hd, line);
		hd->current_line++;
	}
	return (hd->current_line);
}

void	init_hd(t_heredoc *hd, t_redir *redir,
	t_program *program, int pipefd[2])
{
	hd->redir = redir;
	hd->program = program;
	hd->buf = NULL;
	hd->pipefd[0] = pipefd[0];
	hd->pipefd[1] = pipefd[1];
	hd->current_line = 1;
	hd->delimiter = redir->target;
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
	t_heredoc	hd;

	init_hd(&hd, redir, program, pipefd);
	close_fd(&hd.pipefd[0]);
	set_signal_heredoc();
	g_signal_value = 0;
	heredoc_normalize_delimiter(redir);
	hd.current_line = read_heredoc(&hd);
	if (redir->hd_expand && hd.buf)
		expand_heredoc(program, &hd.buf);
	if (hd.buf)
	{
		write(pipefd[1], hd.buf, ft_strlen(hd.buf));
		free(hd.buf);
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
