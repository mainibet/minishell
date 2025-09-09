/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lex_token.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: albetanc <albetanc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/09 07:27:39 by albetanc          #+#    #+#             */
/*   Updated: 2025/09/09 08:29:53 by albetanc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	is_operator_char(char c)
{
	return (c == '|' || c == '&'
		|| c == '>' || c == '<' || c == ';' || c == '(' || c == ')');
}

// Checks multi-character operators
static enum e_toktype	multi_char_operator(char *s)
{
	if (s[0] == '|' && s[1] && s[1] == '|')
		return (OR);
	if (s[0] == '&' && s[1] && s[1] == '&')
		return (AND);
	if (s[0] == '>' && s[1] && s[1] == '>')
		return (APPEND);
	if (s[0] == '<' && s[1] && s[1] == '<')
		return (HEREDOC);
	return (WORD);
}

// Checks single-character operators
static enum e_toktype	single_char_operator(char c)
{
	if (c == '|')
		return (PIPE);
	if (c == ';')
		return (SEMICOLON);
	if (c == '>')
		return (REDIR_OUT);
	if (c == '<')
		return (REDIR_IN);
	if (c == '(')
		return (OPEN);
	if (c == ')')
		return (CLOSE);
	return (WORD);
}

// empty or null string -> treat as WORD
enum e_toktype	token_type(char *s)
{
	enum e_toktype	type;

	if (!s || !*s)
		return (WORD);

	// if (s[0] == '|' && s[1] && s[1] == '|')
	// 	return (OR);
	// if (s[0] == '&' && s[1] && s[1] == '&')
	// 	return (AND);
	// if (s[0] == '>' && s[1] && s[1] == '>')
	// 	return (APPEND);
	// if (s[0] == '<' && s[1] && s[1] == '<')
	// 	return (HEREDOC);

	// Single-character operators
	// if (s[0] == '|')
	// 	return PIPE;
	// if (s[0] == ';')
	// 	return SEMICOLON;
	// if (s[0] == '>')
	// 	return REDIR_OUT;
	// if (s[0] == '<')
	// 	return REDIR_IN;
	// if (s[0] == '(')
	// 	return OPEN;
	// if (s[0] == ')')
	// 	return CLOSE;
	type = multi_char_operator(s);
	if (type != WORD)
		return (type);
	type = single_char_operator(s[0]);
	if (type != WORD)
		return (type);
	// Quotes
	if (s[0] == '\'')
		return (SINGLE_Q);
	if (s[0] == '"')
		return (DOUBLE_Q);
	return (WORD);
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
