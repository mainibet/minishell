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

#include "minishell.h"

// --- FORWARD DECLARATIONS --- //
int	cleanup_cmd_node(t_node *node);
int	cleanup_operator_fd(t_node *node);

void	free_node(t_node *node)
{
	if (node->type == OPERATOR)
	{
		free_node(node->u_data.op.left);
		free_node(node->u_data.op.right);
	}
	free(node);
}

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

//To centralized cleanup
void	cleanup_program(t_program *token_headrogram)
{//probably includes prompt when is dynamic
	if (program->line)
		free(program->line);
	if (program->token_list)
		free_token(program->token_list);
	if (program->root)
		free_node(program->root);
	if (program->envp_cpy)
		free_array(program->envp_cpy);
}

