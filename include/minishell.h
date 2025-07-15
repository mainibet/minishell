/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: albetanc <albetanc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/21 10:47:36 by albetanc          #+#    #+#             */
/*   Updated: 2025/07/15 13:46:22 by albetanc         ###   ########.fr       */
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

// ---FORWARD DECLARATIONS--- //

typedef struct s_token t_token;
typedef struct s_node t_node;

enum e_nodetype
{
	OPERATOR,
	TERMINAL,
	MAX_NODETYPE
};

// -----------------------------------------//
//                 TOKENS                   //
// -----------------------------------------//

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

// -----------------------------------------//
//           TREE NODES TYPES               //
// -----------------------------------------//


// -----------------------------------------//
//             MODULE HEADERS               //
// -----------------------------------------//

# include "libft/libft.h"
# include "include/colors.h"
// # include "include/lexer.h"
// # include "include/parser.h"

#endif
