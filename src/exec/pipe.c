/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: albetanc <albetanc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/04 12:59:38 by albetanc          #+#    #+#             */
/*   Updated: 2025/08/29 13:35:46 by albetanc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"


//uses pipdefd[1] to write

pid_t	execute_left(t_program *program, t_node *left_node, int *pipefd)
{
	pid_t	pid;

	left_node->u_data.cmd.pipefd[0] = pipefd[0];
	left_node->u_data.cmd.pipefd[1] = pipefd[1];
	pid = fork();
	fprintf(stderr, CYAN "Forking for LEFT command: pid = %d\n" RESET, pid);
	if (pid == -1)
	{
		perror("Error: Fork failed for left cmd");
		return (-1);
	}
	else if (pid == 0)
	{
		// Setup redirections for left side before pipe
	       process_redir(&left_node->u_data.cmd, program);
	       // Only connect pipe if no output redirection
	       // If fd_out is still STDOUT_FILENO, use pipe, else use redirected file
	       if (pipefd[1] >= 0 && left_node->u_data.cmd.fd_out == STDOUT_FILENO)
	       {
		       if (dup2(pipefd[1], STDOUT_FILENO) == -1)
		       {
			       perror("Error: dup2 failed for left cmd");
			       exit(1);
		       }
	       }
	       else if (left_node->u_data.cmd.fd_out != STDOUT_FILENO)
	       {
		       if (dup2(left_node->u_data.cmd.fd_out, STDOUT_FILENO) == -1)
		       {
			       perror("Error: dup2 failed for left redir");
			       exit(1);
		       }
	       }
	       close_fd(&pipefd[0]);
	       close_fd(&pipefd[1]);
	       execution(program, left_node, true);
	       exit(EXIT_FAILURE);
	}
	return (pid);
}

pid_t	execute_right(t_program *program, t_node *right_node, int *pipefd)
{
	pid_t	pid;


	right_node->u_data.cmd.pipefd[0] = pipefd[0];
	right_node->u_data.cmd.pipefd[1] = pipefd[1];
	pid = fork();
	fprintf(stderr, CYAN "Forking for RIGHT command: pid = %d\n" RESET, pid);
	if (pid == -1)
	{
		perror("Error: Fork failed for right cmd");
		return (-1);
	}
	else if (pid == 0)
	{
		// if (pipefd[0] >= 0)
	       // If fd_in is still STDIN_FILENO, use pipe, else use redirected file
	       if (right_node->u_data.cmd.fd_in == STDIN_FILENO && pipefd[0] >= 0)
	       {
		       if (dup2(pipefd[0], STDIN_FILENO) == -1)
		       {
			       perror("Error: dup2 failed for right cmd");
			       exit(1);
		       }
		       right_node->u_data.cmd.fd_in = STDIN_FILENO;
	       }
	       else if (right_node->u_data.cmd.fd_in != STDIN_FILENO)
	       {
		       if (dup2(right_node->u_data.cmd.fd_in, STDIN_FILENO) == -1)
		       {
			       perror("Error: dup2 failed for right redir");
			       exit(1);
		       }
	       }
		// close_fd(&pipefd[0]);
		// close_fd(&pipefd[1]);
		execution(program, right_node, true);
		exit(EXIT_FAILURE);
	}
	return (pid);
}

int	close_all_pipefd(int *pipefd_in, int *pipefd_out)
{
	close_fd(pipefd_in);
	close_fd(pipefd_out);
	return (1);
}

bool	has_redir_out(t_redir *redir)//new
{
	while (redir)
	{
		if (redir->type == RED_OUT || redir->type == RED_APPEND)
			return (true);
		redir = redir->next;
	}
	return (false);
}

bool	has_redir_in(t_redir *redir)//new
{
	while (redir)
	{
		if (redir->type == RED_IN || redir->type == RED_HERE_DOC)
			return (true);
		redir = redir->next;
	}
	return (false);
}

void	assign_pipefd(t_node *node, int pipefd[2])
{
	if (!has_redir_out(node->u_data.op.left->u_data.cmd.redir))
	{
		node->u_data.op.left->u_data.cmd.pipefd[1] = pipefd[1];
		node->u_data.op.left->u_data.cmd.fd_out = STDOUT_FILENO; //let the child decide
	}
	else
		node->u_data.op.left->u_data.cmd.pipefd[1] = -1;

	// Right command: use pipe if there are not redir
	if (!has_redir_in(node->u_data.op.right->u_data.cmd.redir))
	{
		node->u_data.op.right->u_data.cmd.pipefd[0] = pipefd[0];
		node->u_data.op.right->u_data.cmd.fd_in = STDIN_FILENO; // let the child decide
	}
	else
		node->u_data.op.right->u_data.cmd.pipefd[0] = -1;
}
//this consider left and right child
//in waitpid 0 makes wait child
//pids[0] is left_pid
//pids[1] is right pid
int	execute_pipeline(t_program *program, t_node *node)
{
	pid_t	pids[2];
	int		pipefd[2];
	int		status;

	if (pipe(pipefd) == -1)
	{
		perror("Error: Pipe failed");
		return (1);
	}
	// node->u_data.op.left->u_data.cmd.pipefd[0] = -1;//new
	// node->u_data.op.left->u_data.cmd.pipefd[1] = pipefd[1];//new
	// node->u_data.op.right->u_data.cmd.pipefd[0] = pipefd[0];//new
	// node->u_data.op.right->u_data.cmd.pipefd[1] = -1;//new
	assign_pipefd(node, pipefd);
	pids[0] = execute_left(program, node->u_data.op.left, pipefd);
	if (pids[0] == -1)//check
	{
		close_all_pipefd(&pipefd[0], &pipefd[1]);
		return (1);
	}
	close_fd(&pipefd[1]);
	pids[1] = execute_right(program, node->u_data.op.right, pipefd);
	if (pids[1] == -1)
	{
		close_fd(&pipefd[0]);
		waitpid(pids[0], NULL, 0);
		return (1);
	}
	close_fd(&pipefd[0]);
	status = wait_children(pids[0], pids[1], &status);
	restore_std(program);
	return (status);
}
