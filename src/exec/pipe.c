#include "minishell.h"
#inclde "exec.h"//temporary for testing

int	setup_pipe(int pipefd[2], int fd_in)//check if static or not
{
	if (pipe(pipefd) == -1)
	{
		perror("pipe failed");
		close_fd(fd_in);
		return (-1);
	}
	return (0);
}