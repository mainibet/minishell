/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redir.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: albetanc <albetanc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/04 12:59:13 by albetanc          #+#    #+#             */
/*   Updated: 2025/07/04 12:59:20 by albetanc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "exec.h" //temporary for testing

int	redir_input(int fd)
{
	int	fd_dup;

	fd_dup = dup2(fd, STDIN_FILENO);
	if (fd_dup == -1)
	{
		perror ("Dup2 in redir_input");
		close_fd (fd);
		return (1);
	}
	close_fd(fd);
	return (0);
}

int	redir_output(int fd)
{
	int	fd_dup;

	fd_dup = dup2(fd, STDOUT_FILENO);
	if (fd_dup == -1)
	{
		perror ("Dup2 in redir_output");
		close_fd(fd);
		return (1);
	}
	close_fd(fd);
	return (0);
}

int	setup_redir(int input_fd, int output_fd, t_fd_dup *dup)
{
	int	fd_in_dup;
	int	fd_out_dup;

	fd_in_dup = redir_input(input_fd);
	if (fd_in_dup < 0)
	{
		perror("Failed redir_input");
		return (-1);
	}
	fd_out_dup = redir_output(output_fd);
	if (fd_out_dup < 0)
	{
		close_fd(fd_in_dup);
		perror("Failed redirection OUTPUT");
		return (-1);
	}
	dup -> input_dup = fd_in_dup;
	dup -> output_dup = fd_out_dup;
	return (0);
}
