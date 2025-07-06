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
*   [ERROR HANDLING]
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


// CREATE A FUNCTION FOR INTERMEDIA PIPES ///

/*
    Used in pipes to count pipe nodes
    1. loop in the nodes until match the type node chosen
    2. count the nodes of that type
    Return the count
	Note: the fisrt if condition is the base case of the recursion
*/
int count_node(t_node *node, t_node_type target_type)//NEW
{
	int	count;

	if (node == NULL) 
		return (0);
	count = 0;
	if (node->type == target_type)
		count = 1;
	count += count_node(node->left, target_type);
	count += count_nodee(node->right, target_type);
	return (count);
}

/*
*   To create a child process using fork()
*   Includes basic error handling for the fork()
*   If success gives control to each child based on child_num
*/

int	fork_handle(t_node *node, t_node_type *type, int i_cmd, int nb_cmd)
{
	pid_t pid;

	pid = fork();
	if (pid == -1)
	{
		perror (BOLD RED "Fork failed" RESET);
		return (cleanup_fd(node, type));//aca el fd_in lo trae el nodo cmd y el pipefd lo trae pipe node
	}
	if (pid == 0)
	{
		if (nb_cmd == 1)
			child_process(node, type);//single cmd
		else if (i_cmd == 0)
			child_first(node, type);
		else if (i_cmd == nb_cmd -1)
			child_last(node, type);
		exit(EXIT_FAILURE);
	}
	return (pid);
}
/*
*   Orchestrates the overall execution flow in the parent process for the current two-command pipex setup
*   1. Get how many cmd are to create same nb of child processes
*/
//check how many childs are: how many pipe nodes, so it will create 2 child pero pipe.
//n cmds need n-1 pipes
//create fork per each child
// int	parent(struct s_pipe_data *data)
int	execute_pipe_node(t_node *root)//this used to be my parent
{
	pid_t    *pid;//save this in a parent struct
	int		child_status;
	int		fork_res;
	int		nb_cmd;
	int		nb_pipes;
	int		i;
	int		j;

	i = 0;
	j = 0;
	nb_cmd = count_node(root, NODE_CMD);
	nb_pipes = count_node(root, NODE_PIPE);
	pid = malloc(nb_cmd);//check
	if (!pid)//check later when is good how we send the correct pid every time
	{
		malloc_error();//check to free all needed and return whats needed
		return (1);
	}
	// --- execute pipe by pipe from root --- //
	while (i < nb_pipes)
	{
		while (j < nb_cmd)
		{
			fork_res = fork_handle(pid, root->pipe, nb_cmd);
			if (check_fork(fork_res, 0, &child_status))
				return (fork_res);
			j++;
		}
		i++;
	}
    close_fd(data -> fd_in);//if exists
	close_fd(data -> pipefd[0]);
	close_fd(data -> pipefd[1]);
	if (wait_child(pid1, &status) == -1 || wait_child(pid2, &status) == -1)//change logic for multiples pipes
	{
		perror ("error waiting child");
		return (-1);
	}
	return (0);
}
