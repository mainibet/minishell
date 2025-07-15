#ifndef PARSE_H
# define PARSE_H
# include <stdlib.h>
# include <string.h>
# include <fcntl.h>
# include <unistd.h>
# include <sys/wait.h>
# include "../lexer/lex.c"

struct s_node;

typedef struct s_operator
{
	enum e_toktype	type;
	struct s_node	*left;
	struct s_node	*right;
}	t_operator;

enum e_nodetype
{
	OPERATOR,
	TERMINAL,
	MAX_NODETYPE
};

union u_node
{
	t_operator	op;
	t_token		*tokens;
};

typedef struct s_node 
{
	enum e_nodetype	type;
	union u_node	content;
}	t_node;

/* prototypes */
/* parse.c */
enum	e_toktype token_type(t_token *token);
t_token	*extract_token(char *s, size_t size);
void	free_token(t_token *token);
char	*consume_whitespace(char *p);
t_token	*lex(char *s, char delim);
int		print_tokens(t_token *token);
t_node	*parse(t_token **token, int min_precedence);
int		exec(t_token *tokens, int fd[3], char **env);
int		traverse(t_node *node, int fd[3], char **env);
#endif
