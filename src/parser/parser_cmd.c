/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_cmd.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: albetanc <albetanc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/08 14:31:48 by albetanc          #+#    #+#             */
/*   Updated: 2025/09/08 18:31:22 by albetanc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	init_cmd_node(t_token *token, t_node *node)
{
	node->type = COMMAND;
	node->u_data.cmd.tokens = token;
	node->u_data.cmd.redir = NULL;
	node->u_data.cmd.argv = NULL;
	node->u_data.cmd.cmd_type = EXECUTABLE;
	node->u_data.cmd.fd_in = STDIN_FILENO;
	node->u_data.cmd.fd_out = STDOUT_FILENO;
	node->u_data.cmd.pipefd[0] = -1;
	node->u_data.cmd.pipefd[1] = -1;
}

static int	validate_redirections(t_token *token)
{
	t_token	*current;

	current = token;
	while (current)
	{
		if (current->type == REDIR_IN || current->type == REDIR_OUT
			|| current->type == APPEND || current->type == HEREDOC)
		{
			if (validate_redir_target(current))
				return (1);
		}
		current = current->next;
	}
	return (0);
}

static int	validate_token_syntax(t_token *token, bool *has_command)
{
	t_token	*current;

	current = token;
	*has_command = false;
	while (current)
	{
		if (current->type == WORD || current->type == SINGLE_Q
			|| current->type == DOUBLE_Q)
		{
			if (validate_word_token(current))
				return (1);
			*has_command = true;
		}
		current = current->next;
	}
	if (validate_redirections(token))
		return (1);
	return (0);
}

int	process_cmd_tokens(t_token *token, t_cmd_data *cmd_data)
{
	t_token	*cmd_tokens;
	bool	has_command;

	if (validate_token_syntax(token, &has_command) != 0)
	{
		cmd_data->tokens = NULL;
		cmd_data->argv = NULL;
		return (1);
	}
	if (process_tokens_loop(token, cmd_data, &cmd_tokens) != 0)
	{
		free_token(cmd_tokens);
		cmd_data->tokens = NULL;
		cmd_data->argv = NULL;
		return (1);
	}
	if (!cmd_tokens && !has_command)
	{
		fprintf(stderr, BOLD RED
			"Syntax error: Command expected but only redirections found\n"
			RESET);
		cmd_data->tokens = NULL;
		cmd_data->argv = NULL;
		return (1);
	}
	cmd_data->tokens = cmd_tokens;
	cmd_data->argv = NULL;
	return (0);
}

