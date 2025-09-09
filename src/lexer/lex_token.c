/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lex_token.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: albetanc <albetanc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/09 07:27:39 by albetanc          #+#    #+#             */
/*   Updated: 2025/09/09 07:36:02 by albetanc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int is_operator_char(char c)
{
	return (c == '|' || c == '&' || c == '>' || c == '<' || c == ';' || c == '(' || c == ')');
}

enum e_toktype token_type(char *s)
{
	if (!s || !*s)
		return WORD; // empty or null string -> treat as WORD

	// Multi-character operators (check length first)
	if (s[0] == '|' && s[1] && s[1] == '|')
		return OR;
	if (s[0] == '&' && s[1] && s[1] == '&')
		return AND;
	if (s[0] == '>' && s[1] && s[1] == '>')
		return APPEND;
	if (s[0] == '<' && s[1] && s[1] == '<')
		return HEREDOC;

	// Single-character operators
	if (s[0] == '|')
		return PIPE;
	if (s[0] == ';')
		return SEMICOLON;
	if (s[0] == '>')
		return REDIR_OUT;
	if (s[0] == '<')
		return REDIR_IN;
	if (s[0] == '(')
		return OPEN;
	if (s[0] == ')')
		return CLOSE;

	// Quotes
	if (s[0] == '\'')
		return SINGLE_Q;
	if (s[0] == '"')
		return DOUBLE_Q;

	// Default fallback
	return WORD;
}

t_token	*extract_token(char *s, size_t size)
{
	t_token	*token;

	token = malloc(sizeof(t_token));
	if (!token)
		return (NULL);
	token->txt = malloc(size + 1);
	if (!token->txt)
	{
		free(token);
		return (NULL);
	}
	strncpy(token->txt, s, size);
	token->txt[size] = 0;
	token->type = token_type(token->txt);
	return (token);
}