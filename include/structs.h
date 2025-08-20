


//PENDING 42 HEADER

#ifndef STRUCTS_H
#define STRUCTS_H

// -----------------------------------------//
//            GLOBAL ENUMS                  //
// -----------------------------------------//

// --- TOKEN TYPES --- //

typedef enum e_toktype
{
	WORD = 1,
	//HEREDOC,
	//REDIR_IN
	//REDIR_OUT
	//APPEND
	OPEN,
	CLOSE,
	PIPE,
	AND,
	OR,
	SEMICOLON,
	MAX_TYPE
}	t_toktype;

// --- COMMAND TYPE --//

typedef enum e_cmdtype
{
	EXECUTABLE,
	BUILTIN,
}	t_cmdtype;

// --- NODE TYPE --- //

typedef enum e_nodetype
{
	OPERATOR,
	COMMAND//CHANGED IT WAS TERMINAL
	// MAX_NODETYPE
}t_nodetype;

// -----------------------------------------//
//                AST STRUCTS               //
// -----------------------------------------//

// --- TOKEN STRUCT --- //

typedef struct s_token
{
	char			*txt;
	enum e_toktype	type;
	struct s_token	*next;
	char			delim;
}	t_token;


// --- COMMAND NODE --- //

typedef struct s_cmd_data//possible include struct with redirections
{
	t_token		*tokens;
	char		**argv;//processed args for execution
	char		**env;
	int			fd_in;//check if this is the strategy
	int			fd_out;//check if this is the strategy
	t_cmdtype	cmd_type;
}	t_cmd_data;

// --- OPERATOR DATA ---//

typedef struct s_operator
{
	enum e_toktype	type;
	struct s_node	*left;
	struct s_node	*right;
}	t_operator;

// --- UNION FOR NODE CONTENT --- //

union u_node_content
{
	t_operator	op;
	t_cmd_data	cmd;
};

// --- STRUCT FOR ALL NODES --- //
typedef struct s_node 
{
	enum e_nodetype			type;
	union u_node_content	u_data;
}	t_node;

typedef struct s_program
{
	char	*line;
	char	**envp;
	char	**envp_cpy;
	t_node	*root;
	t_token	*token_list;
	int		fd_in_orig;//fix pipes fd
	int		fd_out_orig;//fix pipes fd
	int		last_exit_status;
}t_program;

#endif