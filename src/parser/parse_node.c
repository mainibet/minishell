/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_node.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: albetanc <albetanc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/08 16:46:54 by albetanc          #+#    #+#             */
/*   Updated: 2025/09/08 17:51:51 by albetanc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_node	*parse_command(t_token *token)
{
	t_node	*node;

	if (!token)
		return (NULL);
	node = malloc(sizeof(t_node));
	if (!node)
		return (NULL);
	ft_memset(node, 0, sizeof(t_node));
	init_cmd_node(NULL, node);
	if (process_cmd_tokens(token, &node->u_data.cmd) != 0)
	{
		free_node(node);
		DEBUG_ERROR("[ERROR] parse_command: failed to process tokens\n");
		return (NULL);
	}
	return (node);
}

t_node	*parse_operator(t_token *op, t_node *left, t_node *right)
{
	t_node	*node;

	node = malloc(sizeof(t_node));
	if (!node)
	{
		free_node(left);
		free_node(right);
		return (NULL);
	}
	ft_memset(node, 0, sizeof(t_node));
	node->type = OPERATOR;
	node->u_data.op.type = op->type;
	node->u_data.op.left = left;
	node->u_data.op.right = right;
	return (node);
}
