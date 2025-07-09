/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   child.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: albetanc <albetanc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/06 12:07:44 by albetanc          #+#    #+#             */
/*   Updated: 2025/07/09 08:21:22 by albetanc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// #include "minishell.h"
#include "../../minishell.h"

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
void	child_process(t_node *node)
{
	t_fd_dup		dup;
	t_cmd_data		*cmd;

	cmd = &node->u_data.cmd;
	if (setup_redir(cmd->fd_in, cmd->fd_out, &dup) != 0)
		exit(1);
	exec_external_cmd(node);
	perror (BOLD RED "Exec/Builtin failed" RESET);
	cleanup_fd(node, node->type);
	exit(EXIT_FAILURE);
}
