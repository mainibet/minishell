/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parent.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: albetanc <albetanc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/08 16:15:44 by albetanc          #+#    #+#             */
/*   Updated: 2025/07/21 09:46:40 by albetanc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// #include "minishell.h"
#include "../include/minishell.h"
#include "exec.h"//temporary for testing

/**
*   Parent waits for a specific child process to terminate, using waitpid()
*   To clean up after a child and retrieve its exit status
*/
int	wait_one_child(pid_t pid, int *status)
{
	if (pid <= 0)//not wait if there is not valid pid
		return (0);
	if (waitpid(pid, status, 0) == -1)
	{
		perror (BOLD RED "Error waiting for child" RESET);
		return (-1);
	}
	return (0);
}

/**
*   [ERROR HANDLING]
*   @brief Checks the result of a fork_handle
*   If fork() failed, it might wait for a previously launched child 
*   before returning an error status.
*   To managing the parent's flow after a fork attempt.
*/

int	check_fork(int result, pid_t pid, int *status)
{
	if (result != 0)
	{
		if (result == 1)
			return (0);
		else
		{
			if (pid != 0)
				wait_one_child(pid, status);
			return (result);
		}
	}
	return (0);
}

/**
*   @brief Forks a new process
*   1. Calls fork()
*   2. Handles fork errors
*   3. Executes child based on cmd position in a pipeline.
*   4. Exit in case of non sucess in any function to end 
*      correctly child
*   @note This function provides the `pid_t pid` value 
*   by directly calling `fork()`.
*   @note If `pid == -1`, it means fork failed, and 
*   the parent handles the error.
*   @note If `pid == 0`, we are in the child process.
*   @note If `pid > 0`, we are in the parent process.
*/

int	fork_handle(pid_t *pid, t_node *node, int i_cmd, int nb_cmd)
{
	(void) i_cmd;
	*pid = fork();
	if (*pid == -1)
	{
		perror (BOLD RED "Fork failed" RESET);
		cleanup_fd(node, node->type);
		return (-1);
	}
	if (*pid == 0)
	{
		if (nb_cmd == 1)
			child_process(node);
		exit(EXIT_FAILURE);
	}
	return (0);
}

/**
*   [PARENT]
*   @brief handles the execution of a single external cmd
*   Execute single external cmd
*   1. Fork a child process
*   2. Check fork status and handles the errors
*   3. Parent cleanup
*   4. Waits for child
*
*   @return an integer status indicating the child exit status
*   returns -1 on specific parent errors: fork and waitpid
*  
*   @note cleanup_fd before to close fd to so that the child 
*   receives it closed and has more control
*/
int	execute_cmd(t_node *node)
{
	pid_t	pid;
	int		child_status;
	int		fork_status;

	fork_status = fork_handle(&pid, node, 0, 1);
	if (check_fork(fork_status, pid, &child_status))
		return (fork_status);
	cleanup_fd(node, node->type);
	if (wait_one_child(pid, &child_status) == -1)
	{
		perror (BOLD RED "Waitpid failed for child" RESET);
		return (-1);
	}
	if (cleanup_cmd_node(node))
		perror(MAGENTA "Failed to cleanup cmd node\n" RESET);
	cleanup_fd(node, node->type);
	return (0);
}


