#include "minishell.h"
#include "exec.h"//temporary for testing

/*
*   Parent waits for a specific child process to terminate, using waitpid()
*   To clean up after a child and retrieve its exit status
*/
int	wait_child(pid_t pid, int *status)
{
	if (waitpid(pid, status, 0) == -1)
	{
		perror ("Error waiting for child");
		return (-1);
	}
	return (0);
}

/*
*   Checks the result of a fork_handle
*   If fork() failed, it might wait for a previously launched child 
*   before returning an error status.
*   To managing the parent's flow after a fork attempt.
*/

int	check_fork(int result, pid_t pid1, int *status)//may be not needed with multiple child
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
/*
*   To setting up the first command's execution in the pipeline
*   It closes the unused read-end of the pipe
*   Redirects the standard input from data->fd_in (the initial input file) if applies
*   Directs its standard output to the write-end of the pipe (data->pipefd[1])
*   Calls child_process to execute the command
*/
void	child_first(t_pipe_data *data)
{
	t_initial_fd	fd;//check if needed in minishell
	int				child_num;

	child_num = 1;
	close_fd(data -> pipefd[0]);
	fd = (t_initial_fd){data->fd_in, data->pipefd[1]};
	child_process(data, &fd, child_num);
}
/*
*   To setup the second (and final) cmd's execution in the pipeline.
*   Closes the unused write-end of the pipe and the initial input file.
*   Redirects standard input from the read-end of the pipe (data->pipefd[0])
*   Directs its standard output to the newly opened output file.
*   Calls child_process to execute 
*/
void	child_last(t_pipe_data *data)
{
	t_initial_fd	fd;
	int				child_num;
	int				fd_out;

	child_num = 2;
	close(data -> pipefd[1]);
	close(data -> fd_in);
	fd_out = open(data -> argv[data -> argc - 1], O_WRONLY 
			| O_CREAT | O_TRUNC, 0644);//change this logic with multi commands
	if (fd_out == -1)
	{
		perror ("open output file");
		exit (-1);
	}
	fd = (t_initial_fd){data -> pipefd[0], fd_out};
	child_process(data, &fd, child_num);
}

// CREATE A FUNCTION FOR INTERMEDIA PIPES ///

/*
  Used in pipes to count pipe nodes
  1. loop in the nodes until match the type node chosen
  2. count the nodes of that type
  Return the count
*/
int count_node(t_node *node, t_node_type target_type)//NEW
{
	int	count;

    if (node == NULL) 
        return 0; // Base case of the recursion
    count = 0;
    if (node->type == target_type)
        count = 1;
    count += count_node(node->left, target_type);
    count += count_nodee(node->right, target_type);
    return count;
}

/*
*   To create a child process using fork()
*   Includes basic error handling for the fork()
*   If success gives control to each child based on child_num
*/

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
		if (child_num == 1)//CONDITION NEEDS TO BE FIXED WITH MULTIPLES CHILDS
			child_first(data);
		else if (child_num == 2)
			child_last(data);
		return (1);
	}
	return (0);
}
/*
*   Orchestrates the overall execution flow in the parent process for the current two-command pipex setup
*/
int	parent(struct s_pipe_data *data)
{
	pid_t    *pid;//need malloc
	int		status;//to check child
	int		result;//to check fork
    int     nb_child;//to count children
	int		nb_nodes;//to count nodes
	
    result = fork_handle(&pid, data, 1);
	if (check_fork(result, 0, &status))
		return (result);
	close_fd(data -> fd_in);
	close_fd(data -> pipefd[0]);
	close_fd(data -> pipefd[1]);
	if (wait_child(pid1, &status) == -1 || wait_child(pid2, &status) == -1)//change logic for multiples pipes
	{
		perror ("error waiting child");
		return (-1);
	}
	return (0);
}