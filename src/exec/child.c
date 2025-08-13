/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   child.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: albetanc <albetanc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/06 12:07:44 by albetanc          #+#    #+#             */
/*   Updated: 2025/08/06 11:50:12 by albetanc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


// #include "minishell.h"
#include "../include/minishell.h"
#include "../include/exec.h"
#include "../include/minishell.h"
#include "../include/exec.h"
#include "../include/builtin.h"

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
// void	execute_in_child(t_node *node)
void execute_in_child(t_program *program, t_node *node)
{
	// t_fd_dup		dup;
	t_cmd_data		*cmd;
	t_builtin_type	builtin_id;
	int				status;

	cmd = &node->u_data.cmd;
	// if (setup_redir(cmd->fd_in, cmd->fd_out, &dup) != 0)//only when actual redirectio is needed or in pipes
	// 	exit(1);
	if (is_builtin(cmd->argv[0]))//might change if declared $ARG in cmd line
	{
		status = execute_builtin(node);
		exit(status);//some exit status
	}
	else
	{
		exec_external_cmd(node);//this was to execute only single external cmd
		perror (BOLD RED "Exec/Builtin failed" RESET);
		// cleanup_fd(node, node->type);// Only cleanup FDs opened by this child. CHECK IF W OTHERS
		exit(EXIT_FAILURE);
	}
	// if (setup_redir(cmd->fd_in, cmd->fd_out, &dup) != 0)//only when actual redirectio is needed or in pipes
	// 	exit(1);
	exec_external_cmd(node);
	perror (BOLD RED "Exec/Builtin failed" RESET);
	// cleanup_fd(node, node->type);// Only cleanup FDs opened by this child.
	exit(EXIT_FAILURE);
}
