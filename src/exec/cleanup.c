/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cleanup.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: albetanc <albetanc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/06 12:20:44 by albetanc          #+#    #+#             */
/*   Updated: 2025/08/06 10:35:40 by albetanc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// #include "minishell.h"
#include "../include/minishell.h"

// --- FORWARD DECLARATIONS --- //
int	cleanup_cmd_node(t_node *node);
int	cleanup_operator_fd(t_node *node);

/**
*   @brief Closes relevant file descriptors to safely 
*   clean up after a fork failure.
*
*   1. Will identify if is cleaning a pipe_node or a single cmd
*   2. Close_fd will check if there is something to close or not
*   3. @return -1 if close fails
*
*   -Use: e.g. in parent process after execution
*   @note V0: to exectue single external cmd: no pipe no builtins
*/
int	cleanup_fd(t_node *node, t_cmdtype type)
{
	if (!node)
		return (-1);
	if (type == COMMAND)
		cleanup_cmd_node(node);
	else if (type == OPERATOR)//new
		cleanup_operator_fd(node);//new
	return (0);
}

/**
*   @brief Closes tmp fd internal from cmd: fd_in and fd_out
*   
*   1. Checks node validity
*   2. Access cmd data
*   3. Closes fd 
*
*   -Use: call as a part of a broader cleanup routine for cmd exec
*/
int	cleanup_cmd_node(t_node *node)
{
	t_cmd_data	*cmd;

	if (!node || node->type != COMMAND)
		return (-1);
	cmd = &node->u_data.cmd;
	close_fd(node->u_data.cmd.fd_in);
	close_fd(node->u_data.cmd.fd_out);
	return (0);
}

int	cleanup_operator_fd(t_node *node)
{
	if (!node)
		return (-1);
	if (node->type == OPERATOR)
	{
		if (node->u_data.op.left)
			cleanup_operator_fd(node->u_data.op.left);
		if (node->u_data.op.right)
			cleanup_operator_fd(node->u_data.op.right);
	}
	else if (node->type == COMMAND)
		cleanup_cmd_node(node);
	return (0);
}