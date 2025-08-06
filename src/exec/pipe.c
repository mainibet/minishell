/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: albetanc <albetanc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/04 12:59:38 by albetanc          #+#    #+#             */
/*   Updated: 2025/07/31 17:39:55 by albetanc         ###   ########.fr       */
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
// int count_node(t_node *node, t_nodetype target_type)//NEW
// {
// 	int	count;

// 	if (node == NULL) 
// 		return (0);
// 	count = 0;
// 	if (node->type == target_type)
// 		count = 1;
// 	count += count_node(node->u_data.op.left, target_type);
// 	count += count_node(node->u_data.op.right, target_type);
// 	return (count);
// }

/*
	Setup a pipe between commands.
	Returns 0 on success, -1 on failure.
	Closes the input file descriptor if pipe creation fails.
*/
// int	setup_pipe(int pipefd[2], int fd_in)//check if static or not
// {
// 	if (pipe(pipefd) == -1)
// 	{
// 		perror(BOLD RED "Pipe failed\n" RESET);
// 		close_fd(fd_in);
// 		return (-1);
// 	}
// 	return (0);
// }

/*
*   Orchestrates the overall execution flow in the parent process for the current two-command pipex setup
*   1. Get how many cmd are to create same nb of child processes
*/
//check how many childs are: how many pipe nodes, so it will create 2 child pero pipe.
//n cmds need n-1 pipes
//create fork per each child
// int	parent(struct s_pipe_data *data)
// is_pipeline() helper? to mark the cmd?

	// pid_t	pid;
	// int		child_status;
	// int		fork_status;

	// fork_status = fork_handle(&pid, node, 0, 1);
	// if (check_fork(fork_status, pid, &child_status))
	// 	return (fork_status);
	// cleanup_fd(node, node->type);//include condition only for fd bigger than 2
	// if (wait_one_child(pid, &child_status) == -1)
	// return (-1);
pid_t	execute_left(t_node *left_node, int pipefd[2])
{
	pid_t	pid;

	pid = fork();
	if (pid == -1)
	{
		perror(BOLD RED "Error: Fork failed for left cmd" RESET);
		return (-1);
	}
	else if (pid == 0)//child left cmd
	{
		if (dup2(pipefd[1], STDOUT_FILENO) == -1)//uses pipdefd[1] to write
		{
			perror(BOLD RED "Error: dup2 failed for left cmd" RESET);
			exit(1);
		}
		close_fd(pipefd[0]);
		close_fd(pipefd[1]);
		execution(left_node);
		exit(EXIT_FAILURE);
	}
	return (pid);
}

pid_t	execute_right(t_node *right_node, int pipefd[2])
{
	pid_t	pid;

	pid = fork();
	if (pid == -1)
	{
		perror(BOLD RED "Error: Fork failed for right cmd" RESET);
		return (-1);
	}
	else if (pid == 0)//child right cmd
	{
		if (dup2(pipefd[0], STDIN_FILENO) == -1)//uses pipde[0] to read
		{
			perror(BOLD RED "Error: dup2 failed for right cmd" RESET);
			exit(1);
		}
		close_fd(pipefd[0]);
		close_fd(pipefd[1]);
		execution(right_node);
		exit(EXIT_FAILURE);
	}
	return (pid);
}
/*
* Creates pipe (setup_pipe)
* Do forks (check_fork)
* Executes each child in a fork
*/

//this consider left and right child
int	execute_pipeline(t_node *node)
{
	pid_t	left_pid;
	pid_t	right_pid;
	int		pipefd[2];
	int		left_status;
	int		right_status;

	if (pipe(pipefd) == -1)
	{
		perror(BOLD RED "Error: Pipe failed" RESET);
		return (-1);
	}
	left_pid = execute_left(node->u_data.op.left, pipefd);
	if (left_pid == -1)
	{
		close_fd(pipefd[0]);
		close_fd(pipefd[1]);
		return (1);
	}
	right_pid = execute_right(node->u_data.op.right, pipefd);
	if (right_pid == -1)
	{
		close_fd(pipefd[0]);
		close_fd(pipefd[1]);
		waitpid(left_pid, NULL, 0);//0 makes wait child
		return (1);
	}
	close(pipefd[0]);
	close(pipefd[1]);
	return (wait_children(left_pid, right_pid, &right_status));//right as the last one comd to execute
}
