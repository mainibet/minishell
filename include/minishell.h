/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: albetanc <albetanc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/21 10:47:36 by albetanc          #+#    #+#             */
/*   Updated: 2025/07/16 08:25:44 by albetanc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

// -----------------------------------------//
//              COMMON MACROS               //
// -----------------------------------------//

//include them here if needed 
//#define MAX_ARGS 256 //checkif needed
#define BUFFER_SIZE 1024//to read from command line, from fd or from here_doc
                          //check if the buffer size is ok or if we want to chang it

// -----------------------------------------//
//        STANDARD HEADERS (COMMON)         //
// -----------------------------------------//
//STDIO.H FOR: printf, perror
//STDLIB.H FOR: malloc, free, exit, getenv
//UNISTD.H FOR: read, write, access and others
//may be here also later: sys/wait, sys/types: size_t, pid_t, strings.h
# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <sys/wait.h>
# include <readline/readline.h>
# include <readline/history.h>

// -----------------------------------------//
//             MODULE HEADERS               //
// -----------------------------------------//

# include "libft/libft.h"
# include "include/colors.h"
# include "include/lexer.h"
# include "include/parser.h"

// ---FORWARD DECLARATIONS--- //

typedef struct s_token t_token;
typedef struct s_node t_node;

// -----------------------------------------//
//            GLOBAL ENUMS                  //
// -----------------------------------------//

// --- TOKEN TYPES --- //

enum e_toktype
{
	WORD = 1,
	//HEREDOC,
	//FILE_IN,//better REDIR_IN?
	//FILE_OUT,//better REDIR_OUT?
	//APPEND//new
	OPEN,
	CLOSE,
	PIPE,
	AND,
	OR,
	SEMICOLON,
	MAX_TYPE
};

// --- COMMAND TYPE --//

typedef enum e_cmdtype
{
	EXECUTABLE,
	BUILTIN,
}	t_cmdtype;

// --- NODE TYPE --- //

enum e_nodetype
{
	OPERATOR,
	TERMINAL,//can we call it command?
	// MAX_NODETYPE
};

// -----------------------------------------//
//                AST STRUCTS               //
// -----------------------------------------//

// --- COMMAND NODE --- //
typedef struct s_cmd_data
{
	t_token		*tokens;
	char		**argv;//processed args for execution
	char		**env;
	int			fd_in;
	int			fd_out;
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

union u_node
{
	t_operator	op;
	t_cmd_data	cmd;
};

// --- STRUCT FOR ALL NODES --- //
typedef struct s_node 
{
	enum e_nodetype	type;
	union u_node	content;
}	t_node;

// -----------------------------------------//
//                PROTOTYPES                //
// -----------------------------------------//

#endif
