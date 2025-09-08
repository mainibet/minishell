/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_cmd_util.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: albetanc <albetanc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/08 18:11:26 by albetanc          #+#    #+#             */
/*   Updated: 2025/09/08 18:47:42 by albetanc         ###   ########.fr       */
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

int	handle_miss_cmd(t_cmd_data *cmd_data, t_token *cmd_tokens, bool has_cmd)
{
	if (!cmd_tokens && !has_cmd)
	{
		fprintf(stderr, BOLD RED
			"Syntax error: Command expected but only redirections found\n"
			RESET);
		cmd_data->tokens = NULL;
		cmd_data->argv = NULL;
		return (1);
	}
	return (0);
}
