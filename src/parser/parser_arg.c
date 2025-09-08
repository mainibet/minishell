/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_arg.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: albetanc <albetanc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/08 15:07:35 by albetanc          #+#    #+#             */
/*   Updated: 2025/09/08 17:51:44 by albetanc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	error_split_arg(t_token *current)
{
	if (current->next)
		fprintf(stderr, BOLD RED
			"syntax error near unexpected token '%s'\n" RESET,
			current->next->txt);
	else
		fprintf(stderr, BOLD RED
			"syntax error near unexpected token 'newline'\n" RESET);
}

static int	check_next_token(t_token *current)
{
	if (!current->next || (current->next->type != WORD
			&& current->next->type != SINGLE_Q
			&& current->next->type != DOUBLE_Q))
	{
		error_split_arg(current);
		fprintf(stderr, BOLD RED
			"Error: Redirection target missing or invalid.\n" RESET);
		return (1);
	}
	if (current->next->type == REDIR_IN || current->next->type == REDIR_OUT
		|| current->next->type == APPEND || current->next->type == HEREDOC)
	{
		fprintf(stderr, BOLD RED
			"Syntax error: Unexpected redirection operator '%s'\n" RESET,
			current->next->txt);
		return (1);
	}
	return (0);
}

int	split_cmd_arg(t_token **current, t_cmd_data *cmd_data)
{
	t_redir	*tmp_redir;
	bool	quoted;
	int		redir_count;

	if (check_next_token(*current) != 0)
		return (1);
	quoted = ((*current)->next->type == SINGLE_Q
			|| (*current)->next->type == DOUBLE_Q);
	add_redir(&cmd_data->redir,
		create_redir_node((*current)->next->txt,
			map_type((*current)->type), quoted));
	redir_count = 0;
	tmp_redir = cmd_data->redir;
	while (tmp_redir)
	{
		redir_count++;
		tmp_redir = tmp_redir->next;
	}
	*current = (*current)->next->next;
	return (0);
}
