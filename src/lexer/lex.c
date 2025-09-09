/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lex.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: albetanc <albetanc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/15 08:57:29 by albetanc          #+#    #+#             */
/*   Updated: 2025/09/09 17:28:13 by albetanc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*consume_whitespace(char *head_token)
{
	while (*head_token && (*head_token == ' ' || *head_token == '\t'))
		head_token++;
	return (head_token);
}

// Detect multi-char operators
static t_token	*lex_operator(char *s)
{
	size_t	op_len;
	t_token	*token;

	op_len = 1;
	if ((*s == '|' && *(s + 1) == '|') 
		|| (*s == '&' && *(s + 1) == '&') 
		|| (*s == '>' && *(s + 1) == '>') 
		|| (*s == '<' && *(s + 1) == '<'))
		op_len = 2;
	token = extract_token(s, op_len);
	if (!token)
		return (NULL);
	token->type = token_type(token->txt);
	token->next = lex(consume_whitespace(s + op_len), ' ');
	return (token);
}

// Check Quoted string
t_token	*lex(char *s, int delim)
{
	(void) delim;
	if (!s || !*s)
		return (NULL);
	s = consume_whitespace(s);
	if (!*s) 
		return (NULL);
	if (*s == '\'' || *s == '"')
		return (lex_quoted(s + 1, *s));
	else if (is_operator_char(*s))
		return (lex_operator(s));
	else
		return (lex_unquoted(s));
}
