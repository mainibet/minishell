/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lex_quote.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: albetanc <albetanc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/09 07:28:27 by albetanc          #+#    #+#             */
/*   Updated: 2025/09/09 07:47:07 by albetanc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_token	*join_tokens(t_token *a, t_token *b)
{
	if (!a) return b;
	if (!b) return a;

	size_t	len_a = strlen(a->txt);
	size_t	len_b = strlen(b->txt);

	char	*joined = malloc(len_a + len_b + 1);
	if (!joined) 
	{
		free_token(b);
		return (a);
	}
	strcpy(joined, a->txt);
	strcat(joined, b->txt);

	free(a->txt);
	a->txt = joined;

	// preserve the rest of b's chain
	t_token *b_next = b->next;
	free(b->txt);
	free(b);
	a->next = b_next;
	return a;
}


t_token	*lex_quoted(char *s, char quote)
{
	t_token	*token;
	t_token	*next;
	char	*start = s;
	char	*end = s;
	char	*rest;

	while (*end && *end != quote)
	{
		if (quote == '"' && *end == '\\' && (*(end + 1) == '"' || *(end + 1) == '$' || *(end + 1) == '\\'))
			end += 2; // skip escape in double quotes
		else
			end++;
	}
	if (!*end)
	{
		perror("Unclosed quote");
		return (NULL);
	}
	token = extract_token(start, end - start);
	if (!token)
		return (NULL);
	token->type = (quote == '\'') ? SINGLE_Q : DOUBLE_Q;
	rest = end + 1;
	// Check for adjacent token without whitespace
	if (*rest && !ft_isspace(*rest) && !is_operator_char(*rest))
	{
		if (*rest == '\'' || *rest == '"')
			next = lex_quoted(rest + 1, *rest);
		else
			next = lex_unquoted(rest);

		if (!next)
		{
			free_token(token);//new
			return (NULL);
		}

		token = join_tokens(token, next);
	}
	else
		token->next = lex(consume_whitespace(rest), ' ');
	return (token);
}

