/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: albetanc <albetanc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/04 12:59:38 by albetanc          #+#    #+#             */
/*   Updated: 2025/07/29 17:16:59 by albetanc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "exec.h"//temporary for testing

/*
    Used in pipes to count pipe nodes
    1. loop in the nodes until match the type node chosen
    2. count the nodes of that type
    Return the count
	Note: the fisrt if condition is the base case of the recursion
*/
int count_node(t_node *node, t_nodetype target_type)//NEW
{
	int	count;

	if (node == NULL) 
		return (0);
	count = 0;
	if (node->type == target_type)
		count = 1;
	count += count_node(node->u_data.op.left, target_type);
	count += count_node(node->u_data.op.right, target_type);
	return (count);
}

/*
    Setup a pipe between commands.
    Returns 0 on success, -1 on failure.
    Closes the input file descriptor if pipe creation fails.
*/
int	setup_pipe(int pipefd[2], int fd_in)//check if static or not
{
	if (pipe(pipefd) == -1)
	{
		perror(BOLD RED "Pipe failed\n" RESET);
		close_fd(fd_in);
		return (-1);
	}
	return (0);
}

/*
*   Orchestrates the overall execution flow in the parent process for the current two-command pipex setup
*   1. Get how many cmd are to create same nb of child processes
*/
//check how many childs are: how many pipe nodes, so it will create 2 child pero pipe.
//n cmds need n-1 pipes
//create fork per each child
// int	parent(struct s_pipe_data *data)
// is_pipeline() helper? to mark the cmd?

	pid_t	pid;
	int		child_status;
	int		fork_status;

	fork_status = fork_handle(&pid, node, 0, 1);
	if (check_fork(fork_status, pid, &child_status))
		return (fork_status);
	cleanup_fd(node, node->type);//include condition only for fd bigger than 2
	if (wait_one_child(pid, &child_status) == -1)
	return (-1);

    // Un enum para especificar el lado del pipe
typedef enum e_pipe_side
{
	PIPE_LEFT,
	PIPE_RIGHT
}	t_pipe_side;

pid_t	parent_pipe(t_node *node, int *pipe_fd, t_pipe_side)
{
	pid_t	pid;

	fork_status = fork_handle(&pid, node, 0, 1);//fix for multiple cmds, also fork_handle
	if (check_fork(fork_status, pid, &child_status))
		return (fork_status);
	cleanup_fd(node, node->type);//include condition only for fd bigger than 2
	if (wait_one_child(pid, &child_status) == -1)
	return (-1);
	
}
int handle_pipe_node(t_node *node)
{
	int		pipe_fd[2];//for the current node
	pid_t	left_cpid;//left child pid
	pid_t	right_cpid;//right child pid
	int		left_status;
	int		right_status;

	if (set_pipe(pipe_fd == -1))
		return (-1);
	left_cpid = execute_pipe_cmd(node->u_data.op.left, pipe_fd, PIPE_LEFT);
	return (pid);
}

int execute_pipe_node(t_node *node)
{
	pid_t    *pid;//save this in a parent struct
	int		*child_status;
	int		nb_cmd;
	int		nb_pipes;
	int		i;
	// int		j;

	i = 0;
	// j = 0;
	nb_cmd = count_node(node, COMMAND);
	nb_pipes = count_node(node, OPERATOR);//CHECK IF NEEDED WILL CHANGE WITH &&
	if (setup_pipe(node, pipe_fd[1]) == -1)
		return (-1);
	pid = malloc(sizeof(pid_t) * nb_cmd);//check
	if (!pid)//check later when is good how we send the correct pid every time
		return (malloc_error());//check to free all needed and return whats needed
	// --- execute cmds in multiple pipes --- //CHECK IF WORK WITH AST
	pid = NULL;//check
	child_status = malloc(sizeof(int) * nb_cmd);//check
	while (i < nb_cmd)
	{
		pid[i] = fork_handle(pid, node, nb_cmd);//here instead of root will be current node
		if (check_fork(pid[i], 0, &child_status))
			return (pid[i]);//CHECK IF THIS RET IS OK
		i++;
	}
	cleanup_fd(node, node->type);
    // --wait all children --//
    i = 0;//would be usefull with recursion?
	while (i < nb_cmd)//is it possible to have active child check?
	{
		if (wait_child(pid[i], &child_status[i]) == -1)//change logic for multiples pipes
		{
			perror ("error waiting child");
			free_array(pid, i);
			free (pid);
			return (-1);
		}
		i++;
	}
	if (pid)//NEW, check
	{
		free_array(pid, i);//check this function
		free (pid);
	}
	return (0);
}
