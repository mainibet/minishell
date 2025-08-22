/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redir.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: albetanc <albetanc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/04 12:59:13 by albetanc          #+#    #+#             */
/*   Updated: 2025/08/22 13:08:28 by albetanc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_redir	*create_redir_node(char *target, enum e_redir_type type)
{
	t_redir	*new_redir;

	new_redir = malloc(sizeof(t_redir));
	if (!new_redir)
	{
		malloc_error();//check
		return (NULL);
	}
	new_redir->target = ft_strdup(target);//check where to free
	if (!new_redir->target)
	{
		free(new_redir);
		perror("ft_strdup failed redir target");
		return (NULL);
	}
	new_redir->type = type;
	new_redir->fd = -1;//init with a non valid fd
	new_redir->next = NULL;
	return (new_redir);
}

//when target is file name
//only for < > >>
//to set the flags when open redir type
//permissions 0644 due to flag O_CREAT
int	open_redir_filename(t_redir *redir)
{
	int	open_flags;

	if (redir->type == RED_IN)
		open_flags = O_RDONLY;
	else if (redir->type == RED_OUT)
		open_flags = O_WRONLY | O_CREAT | O_TRUNC;
	else if (redir->type == RED_APPEND)
		open_flags = O_WRONLY | O_CREAT | O_APPEND;
	redir->fd = open(redir->target, open_flags, 0644);
	if (redir->fd < 0)
	{
		perror("Error: openfd filed in redir");
		return (1);
	}
	return (0);
}

/**
 * @brief Redirects STDIN_FILENO to a specified fd
*/
static int	redir_input(int fd)
{
	int	result;

	result = dup2(fd, STDIN_FILENO);
	if (result == -1)
	// {
		perror ("Dup2 failed for redir_in");
		// close_fd (fd);//check if is ok onlye closing in setup-redir
	// }
	// close_fd(fd);//check if is ok onlye closing in setup-redir
	return (result);
}

/**
* @brief Redirects STDOUT_FILENO to a specified fd
*/
int	setup_redir(int fd)
{
	int	result;

	result = dup2(fd, STDOUT_FILENO);
	if (result == -1)
	// {
		perror ("Dup2 failed for redir_out");
		// close_fd(fd);//check if is ok only closing in setup-redir
		// return (1);
	// }
	// close_fd(fd);//check if is ok only closing in setup-redir
	return (result);
}

/**
* @brief Sets up both standard input and standard 
* output redirections for a command.
* Uses cmd->fd_in and cmd->fd_out, closes the temporary fds
* after duplicating them to STDIN_FILENO and STDOUT_FILENO.
*/
int	setup_redir(t_cmd_data *cmd)
{
	int	cmd_fd_in;
	int	cmd_fd_out;

	cmd_fd_in = cmd->fd_in;
	cmd_fd_out = cmd ->fd_out;
	if (cmd_fd_in != -1)
	{
		if (redir_input(cmd_fd_in) == -1)
		{
			close_fd(&cmd_fd_in);
			return (perror("Failed redir_input: "), 1);
		}
		close_fd(&cmd_fd_in);//new
	}
	if (cmd_fd_out != -1)
	{
		if (redir_output(cmd_fd_out) == -1)
		{
			close_fd(&cmd_fd_out);
			return (perror("Failed redir_out: "), 1);
		}
		close_fd(&cmd_fd_out);
	}
	return (0);
}

