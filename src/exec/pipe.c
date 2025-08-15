/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: albetanc <albetanc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/04 12:59:38 by albetanc          #+#    #+#             */
/*   Updated: 2025/08/11 14:11:48 by albetanc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

pid_t	execute_left(t_program *program, t_node *left_node, int pipefd[2])
{
	pid_t	pid;

	pid = fork();
	if (pid == -1)
	{
		perror("Error: Fork failed for left cmd");
		return (-1);
	}
	else if (pid == 0)//child left cmd
	{
		if (dup2(pipefd[1], STDOUT_FILENO) == -1)//uses pipdefd[1] to write
		{
			perror("Error: dup2 failed for left cmd");
			exit(1);
		}
		close_fd(pipefd[0]);
		close_fd(pipefd[1]);
		execution(program, left_node, true);
		exit(EXIT_FAILURE);
	}
	return (pid);
}

pid_t	execute_right(t_program *program, t_node *right_node, int pipefd[2])
{
	pid_t	pid;

	pid = fork();
	if (pid == -1)
	{
		perror("Error: Fork failed for right cmd");
		return (-1);
	}
	else if (pid == 0)//child right cmd
	{
		if (dup2(pipefd[0], STDIN_FILENO) == -1)//uses pipde[0] to read
		{
			perror("Error: dup2 failed for right cmd");
			exit(1);
		}
		close_fd(pipefd[0]);
		close_fd(pipefd[1]);
		execution(program, right_node, true);
		exit(EXIT_FAILURE);
	}
	return (pid);
}

//this consider left and right child
int	execute_pipeline(t_program *program, t_node *node)//make it shorter
{
	pid_t	left_pid;
	pid_t	right_pid;
	int		pipefd[2];
	// int		left_status;
	int		right_status;

	if (pipe(pipefd) == -1)
	{
		perror("Error: Pipe failed");
		return (1);
	}
	left_pid = execute_left(program, node->u_data.op.left, pipefd);
	if (left_pid == -1)
	{
		close_fd(pipefd[0]);
		close_fd(pipefd[1]);
		return (1);
	}
	right_pid = execute_right(program, node->u_data.op.right, pipefd);
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
