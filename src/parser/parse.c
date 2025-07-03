typedef struct s_node t_node;

enum e_cmdtype
{
	EXECUTABLE,
	BUILTIN,
	MAX_CMDTYPE
}

typedef struct s_command
{
	char *path;
	char **argv;
	char **env;
	e_cmdtype type;
	int	io[2];
	int	fdother[MAX_OPEN_FILES]; // open fds to close only in child process, -1 terminated
}	t_command;

typedef struct s_operator
{
	e_num	toktype;
	t_node	left;
	t_node	right;
}	t_operator;

enum e_nodetype 
{
	OPERATOR,
	COMMAND,
	MAX_NODETYPE
}

union u_node
{
	t_operator op;
	t_command cmd;
}

typedef struct s_node 
{
	enum e_nodetype type;
	union u_node content;
} t_node;
	
t_token	*next_operator(t_token *token)
{
	while (token->next && token->type < PIPE)
		token = token->next;
	return (token->next);
}

static int	precedence(t_token *token)
{
	if (token->type == SEMICOLON)
		return (1);
	if (token->type == AND || token->type == OR)
		return (2);
	if (token->type == PIPE)
		return (3);
	return (0);
}

/* parse the next command in the token list and return any output file descriptor
 */
int	parse_command(t_token *token, int fdin)
{
	t_command	cmd;
	int			pipe[2];

	cmd.io[0] = fdin;
	cmd.io[1] = 1;
	pipe[0] = 0;
	pipe[1] = 1;
	token = setup_command(token, &cmd); // check for environment setup prefixes
	if (!is_builtin(token->txt))
	{
		token->txt = path_lookup(token->txt); // this must realloc txt
		// error code
	}
	cmd.path = token->txt;
	token = token->next;
	while (token && token->type < PIPE) // until we hit an operator
	{
		process_arg(token, &cmd);
		token = token->next;
	}
	if (token->type == PIPE)
	{
		try_pipe(pipe)
		if (cmd.io[1] == 1) // we didn't find a file output redirect
			cmd.io[1] = pipe[0];
	}
	execute(&cmd);
	return (pipe[1]);
}
	
t_node	*parse(t_token *token, int min_precedence)
{
	t_token	*op;
	int		next_precedence;
	t_node	*node;

	op = next_operator(token);
	next_precedence = precedence(op->type);
	node = malloc(sizeof(t_node));
	if (!node)
		return (NULL);
	node->type = OPERATOR;
	while (op && precedence(op) >= min_precedence)
		op = next_operator(op);
}
