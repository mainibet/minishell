/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: albetanc <albetanc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/12 07:40:52 by albetanc          #+#    #+#             */
/*   Updated: 2025/09/12 07:44:17 by albetanc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ft_print_syntax_error(const char *token)
{
	if (token)
	{
		write(STDERR_FILENO,
			"minishell: syntax error near unexpected token `", 47);
		write(STDERR_FILENO, token, strlen(token));
		write(STDERR_FILENO, "`\n", 2);
	}
	else
		write(STDERR_FILENO,
			"minishell: syntax error near unexpected token `newline'\n", 56);
}


void	ft_print_error(const char *msg)
{
	if (!msg)
		return ;
	write(STDERR_FILENO, msg, ft_strlen(msg));
}
