/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   child_full.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: albetanc <albetanc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/06 12:07:44 by albetanc          #+#    #+#             */
/*   Updated: 2025/07/08 16:27:59 by albetanc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	child_process(t_node *node)
{
	t_fd_dup		dup;
	t_cmd_data		*cmd;

	cmd = &node->u_data.cmd;
	if (setup_redir(cmd->fd_in, cmd->fd_out, &dup) != 0)
		exit(1);
	// if (&node->u_data.cmd == BUILTIN)//later this but if is not in pipe
	// {
	// 	execute_builtin(&node->u_data.cmd);//PENDING THOURGH IF COND WITH STRNCMP
	// 	return (0);//check the return
	// }
	exec_external_cmd(node);//call this from child_process
	perror (BOLD RED "Exec/Builtin failed" RESET);//this will happen if execution fails
	cleanup_fd(node, node->type);
	exit(EXIT_FAILURE);
}

/*
*   Set-up the first cmd's execution in the pipeline
*   1. It closes the unused read-end of the pipe
*   2. Redirects the standard input from data->fd_in (the initial input file) if applies
*   3. Directs its standard output to the write-end of the pipe (data->pipefd[1])
*   4. Calls child_process to execute the command
*/
void	child_first(t_node *node, int prev_pipefd[2], t_initial_fd ini_fd)
{
	close_fd(data -> pipefd[0]);
	fd = (t_initial_fd){data->fd_in, data->pipefd[1]};
	child_process(node, type);
}
/*
*   set-up middle cmds in the execution pipeline
*/
void    child_middle(t_node *node)
{
	int	*fd;//if so need to check that but later (other version)

    fd = (t_initial_fd){data->pipefd[0], data->pipefd[1]};
	child_process(node, type);
}
/*
*   To setup the second (and final) cmd's execution in the pipeline.
*   Closes the unused write-end of the pipe and the initial input file.
*   Redirects standard input from the read-end of the pipe (data->pipefd[0])
*   Directs its standard output to the newly opened output file.
*   Calls child_process to execute 
*/
void	child_last(t_node *node)
{
	int				fd_out;

	close(data -> pipefd[1]);
	close(data -> fd_in);
	fd_out = open(data -> argv[data -> argc - 1], O_WRONLY 
			| O_CREAT | O_TRUNC, 0644);//this later only if there is a redirection in the args
	if (fd_out == -1)
	{
		perror ("open output file");
		exit (-1);
	}
	fd = data -> pipefd[0], fd_out;
	child_process(node, type);
}