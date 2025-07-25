/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cleanup_full.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: albetanc <albetanc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/06 12:20:44 by albetanc          #+#    #+#             */
/*   Updated: 2025/07/25 11:05:36 by albetanc         ###   ########.fr       */
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
// int	cleanup_fd(t_node *node, t_node_type type)//included in cleanup.c
// {
// 	if (!node)
// 		return (-1);
// 	if (type == NODE_CMD)
//         cleanup_cmd_node(node);
// 	// {
// 	// 	close_fd(&node->u_data.cmd.fd_in);
// 	// 	close_fd(&node->u_data.cmd.fd_out);
// 	// }
// 	if (type == NODE_PIPE)
// 		cleanup_pipe_node(node);
// 	// {
// 	// 	close_fd(&node->u_data.pipe.input_fd);
// 	// 	close_fd(&node->u_data.pipe.output_fd);
// 	// }
// 	return (-1);
// }

/*
* Closes tmp fd internal from cmd: fd_in and fd_out
// */
// void	cleanup_cmd_node(t_node *node)//al ready in cleanup.c
// {
// 	t_cmd_data	*cmd;

// 	if (!node || node->type != NODE_CMD)
// 		return ;
// 	cmd = &node->u_data.cmd;
// 	close_fd(&node->u_data.cmd.fd_in);
// 	close_fd(&node->u_data.cmd.fd_out);
// }

// void	cleanup_pipe_fd(t_node *node)//included in cleanup.c
// {
// 	t_pipe_data	*pipe;

// 	pipe = &node->u_data.pipe;
// 	close_fd(&pipe->input_fd);
// 	close_fd(&pipe->output_fd);
// }
