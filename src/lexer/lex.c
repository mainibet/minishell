/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lex.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: albetanc <albetanc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/15 08:57:29 by albetanc          #+#    #+#             */
/*   Updated: 2025/07/21 12:05:40 by albetanc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

enum e_toktype	token_type(t_token *token)
{
	// if (*token->txt == '(' || *token->txt == '{')
	// 	return (OPEN);
	// if (*token->txt == ')' || *token->txt == '}')
	// 	return (CLOSE);
	if (*token->txt == '|' && !*(token->txt + 1))
		return (PIPE);
	// if (*token->txt == '&' && *(token->txt + 1) == '&')
	// 	return (AND);
	if (*token->txt == '|' && *(token->txt + 1) == '|')
		return (OR);
	// if (*token->txt == ';')
	// 	return (SEMICOLON);
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
	token->type = token_type(token);
	return (token);
}

char	*consume_whitespace(char *head_token)
{
	while (*head_token && (*head_token == ' ' || *head_token == '\t'))
		head_token++;
	return (head_token);
}

t_token	*lex(char *s, char delim)//make it shorter
{
	char	*head_token;
	char	*current;
	t_token	*token;

	head_token = s;
	current = head_token;
	while (*current && *current!= delim)
		current++;
	if (!*current&& delim != ' ')
	{
		perror("unclosed quote");
		return (NULL);
	}
	token = extract_token(head_token, current - head_token);
	if (!token)
		return (NULL);
	token->delim = delim;
	current = consume_whitespace(current);
	if (*current)
	{
		if (*current== '\'' || *current== '"')
			token->next = lex(current + 1, *current);
		else
			token->next = lex(current, ' ');
		if (token->next == NULL && *current)
		{
			free(token->txt);
			free(token);
			return (NULL); 
		}
	}
	else
		token->next = NULL;
		return (token);
}

int	print_tokens(t_token *token)
{
	int	len;

	len = 0;
	while (token)
	{
		len += printf("%s ", token->txt);
		token = token->next;
	}
	return (len);
}
