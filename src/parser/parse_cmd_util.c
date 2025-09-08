/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_cmd_util.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: albetanc <albetanc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/08 18:11:26 by albetanc          #+#    #+#             */
/*   Updated: 2025/09/08 18:32:37 by albetanc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	validate_word_token(t_token *token)
{
	if (token->type == WORD && (ft_strchr(token->txt, '>') != NULL
			|| ft_strchr(token->txt, '<') != NULL))
	{
		fprintf(stderr, BOLD RED
			"Syntax error: Invalid token '%s' contains "
			"redirection characters\n" RESET, token->txt);
		return (1);
	}
	return (0);
}

int	validate_redir_target(t_token *current)
{
	if (!current->next)
	{
		fprintf(stderr, BOLD RED
			"Syntax error: Missing redirection target after '%s'\n"
			RESET, current->txt);
		return (1);
	}
	if (current->next->type == REDIR_IN
		|| current->next->type == REDIR_OUT
		|| current->next->type == APPEND
		|| current->next->type == HEREDOC)
	{
		fprintf(stderr, BOLD RED
			"Syntax error: Unexpected redirection operator '%s'"
			" after '%s'\n" RESET, current->next->txt, current->txt);
		return (1);
	}
	return (0);
}
