#include <stdio.h>
#include <stdlib.h>
#include <string.h>

enum e_toktype
{
	WORD = 1,
	//HEREDOC,
	//FILE_IN,
	//FILE_OUT,
	OPEN,
	CLOSE,
	PIPE,
	AND,
	OR,
	SEMICOLON,
	MAX_TYPE
};

typedef struct s_token t_token;

typedef struct s_token
{
	char	 		*txt;
	t_token			*next;
	enum e_toktype	type;
	char			delim;
}	t_token;

/* advance the token pointer to the beginning of the next token */
/* 
char *consume_token(char *s)
{
	if (*s == '"' || *s == '\'')
	{
		s = strchr(s + 1, *s);
		s++;
	}
	else
		s = strchr(s, ' ');
	return (s);
}

static int	size_quoted(char *s, t_token *token)
{
	int size;

	token->delim = *s;
	size = strchr(s + 1, *s) - s - 1;
	if (size < 0) // no closing quote; should never happen
	{
		free(token);
		exit(1);
	}
	return (size);
}

static int	size_word(char *s, t_token *token)
{
	int 	size;
	char	*end;

	token->delim = ' ';
	end = strchr(s, ' ');
	if (end)
		size = end - s;
	else
		size = strlen(s);
	return (size);
}

void	process_token(t_token token)
{
	// expand variables, path commands, set type, etc.
}
*/

enum e_toktype token_type(t_token *token)
{
	if (*token->txt == '(' || *token->txt == '{')
		return (OPEN);
	if (*token->txt == ')' || *token->txt == '}')
		return (CLOSE);
	if (*token->txt == '|' && !*(token->txt + 1))
		return (PIPE);
	if (*token->txt == '&' && *(token->txt + 1) == '&')
		return (AND);
	if (*token->txt == '|' && *(token->txt + 1) == '|')
		return (OR);
	if (*token->txt == ';')
		return (SEMICOLON);
	return (WORD);
}

/* take a pointer to the beginning of a token in a string and extract the whole 
 * token.  If the first character is a quote, the whole quoted strding is 
 * the token, otherwise the _word_ is the token
 */
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
	strncpy(token->txt, s, size);
	token->txt[size] = 0;
	token->type = token_type(token);
	return (token);
}

void free_token(t_token *token)
{
	if (token->next)
		free_token(token->next);
	free(token->txt);
	free(token);
}

char	*consume_whitespace(char *p)
{
	while (*p && (*p == ' ' || *p == '\t'))
		p++;
	return (p);
}

t_token *lex(char *s, char delim)
{
	char	*p;
	char	*q;
	t_token *token;

	p = s;
	q = p;
	// push q to the end of the token
	while (*q && *q != delim)
		q++;
	if (!*q && delim != ' ')
	{
		perror("unclosed quote");
		exit(1);
	}
	token = extract_token(p, q - p);
	if (!token)
		return (NULL);
	// so we know whether to expand variables
	token->delim = delim;
	q = consume_whitespace(q);
	if (*q)
		if (*q == '\'' || *q == '"')
			token->next = lex(q + 1, *q);
		else
			token->next = lex(q, ' ');
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
