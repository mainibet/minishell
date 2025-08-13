/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parent.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: albetanc <albetanc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/08 16:15:44 by albetanc          #+#    #+#             */
/*   Updated: 2025/08/11 14:13:21 by albetanc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// #include "minishell.h"
#include "../include/minishell.h"
#include "exec.h"//temporary for testing

int	wait_children(t_program *program, pid_t left_pid, pid_t right_pid, int *right_status)//NEW
{
	int	left_status;

	(void) program; //check
	waitpid(left_pid, &left_status, 0);
	waitpid(right_pid, right_status, 0);
	if (WIFEXITED(*right_status))
		return (WEXITSTATUS(*right_status));
	return (1);
}

/**
*   Parent waits for a specific child process to terminate, using waitpid()
*   To clean up after a child and retrieve its exit status
*/
// int	wait_one_child(pid_t pid, int *status)
// {
// 	if (pid <= 0)//not wait if there is not valid pid
// 		return (0);
// 	if (waitpid(pid, status, 0) == -1)
// 	{
// 		perror (BOLD RED "Error waiting for child" RESET);
// 		return (-1);
// 	}
// 	return (0);
// }

/**
*   [ERROR HANDLING]
*   @brief Checks the result of a fork_handle
*   If fork() failed, it might wait for a previously launched child 
*   before returning an error status.
*   To managing the parent's flow after a fork attempt.
*/

// int	check_fork(int result, pid_t pid, int *status)
// {
// 	if (result != 0)
// 	{
// 		if (result == 1)
// 			return (0);
// 		else//NEW
// 		{
// 			if (pid != 0)
// 				// wait_one_child(pid, status);
//                 wait_child();//TO DO FUNCION to choose if child 1 or children
// 			return (result);
// 		}
// 	}
// 	return (0);
// }

// /**
// *   @brief Forks a new process
// *   1. Calls fork()
// *   2. Handles fork errors
// *   3. Executes child based on cmd position in a pipeline.
// *   4. Exit in case of non sucess in any function to end 
// *      correctly child
// *   @note This function provides the `pid_t pid` value 
// *   by directly calling `fork()`.
// *   @note If `pid == -1`, it means fork failed, and 
// *   the parent handles the error.
// *   @note If `pid == 0`, we are in the child process.
// *   @note If `pid > 0`, we are in the parent process.
// */

// int	fork_handle(pid_t *pid, t_node *node, int i_cmd, int nb_cmd)
// {
// 	(void) i_cmd;
// 	*pid = fork();
// 	if (*pid == -1)
// 	{
// 		perror (BOLD RED "Fork failed" RESET);
// 		cleanup_fd(node, node->type);
// 		return (-1);
// 	}
// 	if (*pid == 0)
// 	{
// 		if (nb_cmd == 1)
// 			child_process(node);//change for pipes (multiple cmd)
// 		else
			
//         exit(EXIT_FAILURE);
// 	}
// 	return (0);
// }

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
//to execute cmd witout pipes so need fork
// int	execute_external_cm(t_node *node)
// {
// 	pid_t	pid;
// 	int		child_status;
// 	int		fork_status;

// 	fork_status = fork_handle(&pid, node, 0, 1);
// 	if (check_fork(fork_status, pid, &child_status))
// 		return (fork_status);
// 	cleanup_fd(node, node->type);//include condition only for fd bigger than 2
// 	if (wait_one_child(pid, &child_status) == -1)
// 	return (-1);
// 	// if ((node->u_data.cmd.fd_in != 0) && (cleanup_cmd_node(node)))//this generates error in external cmd because we include 0 and 1 in cmd node
//     //if is not stdin
// 		// perror(MAGENTA "Failed to cleanup cmd node\n" RESET);
// 	// cleanup_fd(node, node->type);//include condition only for fd bigger than 2
// 	return (0);//migth change for the child's actual exit status
// }

//new version to include pipes
//fork before execve
//Used when cmd is not in pipe
int	exec_cmd_nopipe(t_program *program, t_node *node)
{
	pid_t	pid;
	int		child_status;

	pid = fork();
	if (pid == -1)
	{
		perror(BOLD RED "Fork failed for top-level command" RESET);
		return (1);
	}
	else if (pid == 0)//child
	{
		//CHECK FD TO CLOSE
		child_process(program, node);
		exit(EXIT_FAILURE);
	}
	else // Parent process
	{
		waitpid(pid, &child_status, 0);
		if (WIFEXITED(child_status))
			return (WEXITSTATUS(child_status));
		return (1); // Default error if not exited normally
	}
}
