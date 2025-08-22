/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redir_util.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: albetanc <albetanc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/22 13:33:32 by albetanc          #+#    #+#             */
/*   Updated: 2025/08/22 15:20:16 by albetanc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// To update the command's file descriptors.
static void	update_redir_fd(int new_fd, int *cmd_fd)
{
	if (*cmd_fd != STDIN_FILENO && *cmd_fd != STDOUT_FILENO)
		close_fd(cmd_fd);
	*cmd_fd = new_fd;
}
//iterates over the command’s linked list of redirections
//to open target files and set cmd_fdin and cmd_fdout 
//after all redir
// 1. Open the file and check for errors immediately.
// If open fails, close all previously opened FDs.
// 3. Close the previous output FD if it's not STDOUT.
int	process_redir(t_cmd_data *cmd)
{
	t_redir	*current_redir;

	current_redir = cmd->redir;
	cmd->fd_in = STDIN_FILENO;
	cmd->fd_out = STDOUT_FILENO;
	while (current_redir)
	{
		if (open_redir_filename(current_redir) != 0)
		{
			if (cmd->fd_in != STDIN_FILENO)
				close_fd(&cmd->fd_in);
			if (cmd->fd_out != STDOUT_FILENO)
				close_fd(&cmd->fd_out);
			return (1);
		}
		if (current_redir->type == RED_IN)
			update_redir_fd(current_redir->fd, &cmd->fd_in);
		else
			update_redir_fd(current_redir->fd, &cmd->fd_out);
		current_redir = current_redir->next;
	}
	return (0);
}
