/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   child.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: albetanc <albetanc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/06 12:07:44 by albetanc          #+#    #+#             */
/*   Updated: 2025/07/06 12:29:50 by albetanc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	child_process(t_node *node, t_node_type *type)
{
	t_fd_dup	dup;

	if (setup_redir(fd -> input_fd, fd -> output_fd, &dup) != 0)
		exit(1);
	execution(node);
	perror (BOLD RED "Execution failed in child 1" RESET);
	cleanup_fd(node, type);
	exit(1);
}

/*
*   Set-up the first cmd's execution in the pipeline
*   1. It closes the unused read-end of the pipe
*   2. Redirects the standard input from data->fd_in (the initial input file) if applies
*   3. Directs its standard output to the write-end of the pipe (data->pipefd[1])
*   4. Calls child_process to execute the command
*/
void	child_first(t_node *node, t_node_type *type)
{
	t_initial_fd	fd;//adapt to AST

	close_fd(data -> pipefd[0]);
	fd = (t_initial_fd){data->fd_in, data->pipefd[1]};
	child_process(node, type);
}
/*
*   set-up middle cmds in the execution pipeline
*/
void    child_middle(t_node *node, t_node_type *type)
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
void	child_last(t_node *node, t_node_type *type)
{
	t_initial_fd	fd;//this comes in the node
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
	fd = (t_initial_fd){data -> pipefd[0], fd_out};
	child_process(node, type);
}