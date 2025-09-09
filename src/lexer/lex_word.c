/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lex_word.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: albetanc <albetanc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/09 07:28:43 by albetanc          #+#    #+#             */
/*   Updated: 2025/09/09 08:20:28 by albetanc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_isspace(char c)
{
	return (c == ' ' || c == '\t' || c == '\n'
		|| c == '\v' || c == '\f' || c == '\r');
}

t_token	*lex_unquoted(char *s)
{
	char	*start = s;
	char	*end = s;
	char	*rest;
	t_token	*token;
	t_token	*next;

	while (*end && !ft_isspace(*end) && !is_operator_char(*end) && *end != '\'' && *end != '"')
	{
		if (*end == '\\' && *(end + 1)) // handle escape
		end += 2;
		else
			end++;
	}
	token = extract_token(start, end - start);
	if (!token)
		return (NULL);
	token->type = WORD;
	// Check for redirection characters within the word
	if (strchr(token->txt, '>') != NULL || strchr(token->txt, '<') != NULL) {
		// If the word contains redirection chars, check if they're at positions where
		// they'd form valid redirection syntax (beginning of word or after space)
		for (size_t i = 0; token->txt[i]; i++)
		{
		if ((token->txt[i] == '>' || token->txt[i] == '<') && i > 0 && token->txt[i-1] != ' ') 
			{
				fprintf(stderr, BOLD RED "Syntax error: Invalid redirection syntax in '%s'\n" RESET, token->txt);
				free_token(token);
				return (NULL);
			}
		}
	}
	rest = end;
	// Check for adjacent token without whitespace
	if (*rest && !ft_isspace(*rest) && !is_operator_char(*rest))
	{
		if (*rest == '\'' || *rest == '"')
			next = lex_quoted(rest + 1, *rest);
		else
			next = lex_unquoted(rest);
		if (!next)
		{
			free_token(token);
			return (NULL);
		}
		token = join_tokens(token, next);
	}
	else
		token->next = lex(consume_whitespace(rest), ' ');
	return (token);
}
