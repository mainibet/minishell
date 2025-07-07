/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cleanup.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: albetanc <albetanc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/06 12:20:44 by albetanc          #+#    #+#             */
/*   Updated: 2025/07/07 07:35:54 by albetanc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	free_memory(char **narg, int j)
{
	while (j > 0)
	{
		free (narg[j - 1]);
		j--;
	}
	free (narg);
}
/*
*   Closes relevant file descriptors to safely clean up after a fork failure.
*   1. Will identify if is cleaning a pipe_node or a single cmd
*   2. Close_fd will check if there is something to close or not
*   3. Return -1 if close fails
*/
int	cleanup_fd(t_node *node, t_node_type *type)
{
	if (!node)
		return (-1);
	if (type == NODE_CMD)
	{
		close_fd(&node->u_data.cmd.fd_in);
		close_fd(&node->u_data.cmd.fd_out);
	}
	if (type == NODE_PIPE)
	{
		close_fd(&node->u_data.pipe.input_fd);
		close_fd(&node->u_data.pipe.output_fd);
	}
	return (-1);
}

void	cleanup_cmd_node(t_node *NODE_CMD);
{
	close_fd(NODE_CMD->io[0]);
	close_fd(NODE_CMD->io[1]);
}
