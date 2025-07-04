/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: albetanc <albetanc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/04 12:59:38 by albetanc          #+#    #+#             */
/*   Updated: 2025/07/04 12:59:46 by albetanc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "exec.h"//temporary for testing

/*
    Setup a pipe between commands.
    Returns 0 on success, -1 on failure.
    Closes the input file descriptor if pipe creation fails.
*/
int	setup_pipe(int pipefd[2], int fd_in)//check if static or not
{
	if (pipe(pipefd) == -1)
	{
		perror("pipe failed");
		close_fd(fd_in);
		return (-1);
	}
	return (0);
}