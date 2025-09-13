/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lex_word.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: albetanc <albetanc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/09 07:28:43 by albetanc          #+#    #+#             */
/*   Updated: 2025/09/12 08:07:45 by albetanc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_isspace(char c)
{
	return (c == ' ' || c == '\t' || c == '\n'
		|| c == '\v' || c == '\f' || c == '\r');
}

// Advance to the end of an unquoted token
// handle escape
static char	*scan_unquoted_end(char *end)
{
	while (*end && !ft_isspace(*end) && !is_operator_char(*end)
		&& *end != '\'' && *end != '"')
	{
		if (*end == '\\' && *(end + 1))
			end += 2;
		else
			end++;
	}
	return (end);
}

// Check for malformed redirection within the token
static int	check_redir_syntax(t_token	*token)
{
	size_t	i;

	i = 0;
	while (token->txt[i])
	{
		if ((token->txt[i] == '>' || token->txt[i] == '<')
			&& i > 0 && token->txt[i - 1] != ' ') 
		{
			ft_print_syntax_error(token->txt);
			free_token(token);
			return (0);
		}
		i++;
	}
	return (1);
}

// Process adjacent token without space
static t_token	*handle_adj_token(t_program *program, t_token *token, char *rest)
{
	t_token	*next;

	if (*rest == '\'' || *rest == '"')
		next = lex_quoted(program, rest + 1, *rest);
	else
		next = lex_unquoted(program, rest);
	if (!next)
	{
		free_token(token);
		return (NULL);
	}
	return (join_tokens(token, next));
}

// Check for redirection characters within the word
// If the word contains redirection chars, check if they're at positions where
// they'd form valid redirection syntax (beginning of word or after space)
// Check for adjacent token without whitespace
t_token	*lex_unquoted(t_program *program, char *s)
{
	char	*start;
	char	*end;
	char	*rest;
	t_token	*token;

	start = s;
	end = scan_unquoted_end(s);
	token = extract_token(start, end - start);
	if (!token)
		return (NULL);
	token->type = WORD;
	add_token(&program->token_list, token);
	if (strchr(token->txt, '>') != NULL || strchr(token->txt, '<') != NULL)
	{
		if (!check_redir_syntax(token))
			return (NULL);
	}
	rest = end;
	if (*rest && !ft_isspace(*rest) && !is_operator_char(*rest))
		return (handle_adj_token(program, token, rest));
	// else
	// 	token->next = lex(consume_whitespace(rest), program);
	return (token);
}
