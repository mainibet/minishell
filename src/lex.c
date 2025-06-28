#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct s_token t_token;

typedef struct s_token
{
	char 	*txt;
	t_token	*next;
}	t_token;

t_token *extract_token(char *s, size_t size)
{
	t_token *token;
	token = malloc(sizeof(t_token));
	if (!token)
		return (NULL);
	token->txt = malloc(size + 1);
	if (!token->txt)
	{
		free(token);
		return (NULL);
	}
	memcpy(token->txt, s, size);//change later for ft_memcpy
	token->txt[size] = 0;
	return (token);
}

void free_token(t_token *token)
{
	if (token->next)
		free_token(token->next);
	free(token->txt);
	free(token);
}

t_token *lex(char *s)
{
	char	*p;
	char	*q;
	t_token *token;

	p = s;
	while (*p && *p == ' ')
		p++;
	q = p;
	while (*q && *q != ' ')
		q++;
	token = extract_token(p, q - p);
	if (*q)
		token->next = lex(q);
	else
		token->next = NULL;
	return (token);
}

int print_tokens(t_token *token)
{
	int len = 0;
	while (token)
	{
		len += printf("%s ", token->txt);
		token = token->next;
	}
	return (len);
}
