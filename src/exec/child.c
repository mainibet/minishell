/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   child.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: albetanc <albetanc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/06 12:07:44 by albetanc          #+#    #+#             */
/*   Updated: 2025/08/26 07:48:37 by albetanc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "minishell.h"

/**
*   @brief sets up I/O redi and executes cmd
*
*   1. Call set-up redirections
*   2. Call exec_external_cmd
*   3. Handles if execution fails
*   4. @return exit
*
*   - use: only within forked childs
*
*   @note v0: execute single external cmd: no pipes no builtins
*/
// void	child_process(t_node *node)
void	child_process(t_program *program, t_node *node)
{
	t_cmd_data		*cmd;
	// t_builtin_type	builtin_id;
	int				status;

	cmd = &node->u_data.cmd;
	// int redir_count = 0;
	// t_redir *tmp_redir = cmd->redir;
	// while (tmp_redir) { redir_count++; tmp_redir = tmp_redir->next; }
	// fprintf(stderr, "\033[1;36m[DEBUG] child_process: cmd->redir is %s (%d redirs)\033[0m\n", cmd->redir ? "NOT NULL" : "NULL", redir_count);
	if (cmd->redir)
	{
		// if (!is_pipe_child)//new
		// {//new
		// 	if (process_redir(cmd, program) != 0)
		// 		exit(EXIT_FAILURE);
		// }//new
		// if (node->u_data.cmd.pipefd[0] > 2)
		// 	node->u_data.cmd.fd_in = node->u_data.cmd.pipefd[0];
		// if (node->u_data.cmd.pipefd[1] > 2)
		// 	cmd->fd_out = cmd->pipefd[1];
		if (setup_redir(cmd) != 0)//new handling error
			exit(EXIT_FAILURE);
	}
	if (is_builtin(cmd->argv[0]))//might change if declared $ARG in cmd line
	{
		status = execute_builtin(program, node, true);
		exit(status);//some exit status
	}
	else
	{
		exec_cmd_inchild(node);//this was to execute only single external cmd
		perror (BOLD RED "Exec/Builtin failed" RESET);
		// cleanup_fd(node, node->type);// Only cleanup FDs opened by this child. CHECK IF W OTHERS
		exit(EXIT_FAILURE);
	}
}
