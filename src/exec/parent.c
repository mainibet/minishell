#include "minishell.h"
#include "exec.h"//temporary for testing

int	wait_child(pid_t pid, int *status)
{
	if (waitpid(pid, status, 0) == -1)
	{
		perror ("Error waiting for child");
		return (-1);
	}
	return (0);
}

int	fork_handle(pid_t *pid, t_pipe_data *data, int child_num)
{
	*pid = fork();
	if (*pid == -1)
	{
		perror ("Fork failed");
		return (fork_error(data->fd_in, data->pipefd));
	}
	if (*pid == 0)
	{
		if (child_num == 1)
			child1(data);
		else if (child_num == 2)
			child2(data);
		return (1);
	}
	return (0);
}

int	check_fork(int result, pid_t pid1, int *status)
{
	if (result != 0)
	{
		if (result == 1)
			return (0);
		else
		{
			if (pid1 != 0)
				wait_child(pid1, status);
			return (result);
		}
	}
	return (0);
}

void	child1(t_pipe_data *data)
{
	t_initial_fd	fd;
	int				child_num;

	child_num = 1;
	close_fd(data -> pipefd[0]);
	fd = (t_initial_fd){data->fd_in, data->pipefd[1]};
	child_process(data, &fd, child_num);
}

void	child2(t_pipe_data *data)
{
	t_initial_fd	fd;
	int				child_num;
	int				fd_out;

	child_num = 2;
	close(data -> pipefd[1]);
	close(data -> fd_in);
	fd_out = open(data -> argv[data -> argc - 1], O_WRONLY 
			| O_CREAT | O_TRUNC, 0644);
	if (fd_out == -1)
	{
		perror ("open output file");
		exit (-1);
	}
	fd = (t_initial_fd){data -> pipefd[0], fd_out};
	child_process(data, &fd, child_num);
}

static int	parent(struct s_pipe_data *data)
{
	pid_t	pid1;
	pid_t	pid2;
	int		status;
	int		result;

	result = fork_handle(&pid1, data, 1);
	if (check_fork(result, 0, &status))
		return (result);
	result = fork_handle(&pid2, data, 2);
	if (check_fork(result, pid1, &status))
		return (result);
	close_fd(data -> fd_in);
	close_fd(data -> pipefd[0]);
	close_fd(data -> pipefd[1]);
	if (wait_child(pid1, &status) == -1 || wait_child(pid2, &status) == -1)
	{
		perror ("error waiting child");
		return (-1);
	}
	return (0);
}