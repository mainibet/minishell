/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: albetanc <albetanc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/21 11:02:07 by albetanc          #+#    #+#             */
/*   Updated: 2025/07/09 14:42:08 by albetanc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXEC_H
# define EXEC_H

// --- STANDARD HEADERS ---
#include <readline/readline.h>
#include <readline/history.h>

// ---FORWARD DECLARATIONS--- //
typedef struct s_tree_node t_node;

//  --- COMMAND TYPE --- //

/**
 *  @brief Defines classification of a cmd
 * 
 * - Use: to recognize a external cmd froma builtin
 * 
 *  @note later might be use MAX_CMDTYPE to control
 *  precedency
*/
typedef enum e_cmdtype
{
	EXECUTABLE,
	BUILTIN,
}	t_cmdtype;

// --- INITIAL VERSION CMD NODE --- // 
/**
*   @brief stores data relevand in cmd_node AST:
*   single external cmd
*
*   @note This struct intend to be part of `u_data' 
*   union in AST
*   @note `argv` will be later `t_token *tokens`
*   @note future version might have io[2] instead of 
*   @note future veresion might have int fd_other[MAX_OPEN_FDs]
*   if so they will need to be closed before child 
*   if not needed fd_in fd_out received
*/
typedef struct s_cmd_data
{
    char	**argv;
    char	**env;
    int		fd_in;
	int		fd_out;
	t_cmdtype	cmd_type;
} t_cmd_data;


/**
*   @brief pipe operrations connecting 2 cmd branches 
*   in AST
*
*   Pipe node will hold 2 pointers to left and right 
*   child nodes
*/
typedef struct s_pipe_data
{
	t_tree_node  *left;
	t_tree_node  *right;
} t_pipe_data;

// -----------------------------------------//
//           TREE NODES TYPES               //
// -----------------------------------------//

/**
*   @brief defines types of nodes in AST
*
*   1. Categorizes the different structural elements
*      a parsed cmd line can represent
*   @usage for directing execution flow
*   
*   @note This enum was initially created 
*   for testing the execution logic.
*/
typedef enum e_node_type//CREATED FOR TESTING EXEC
{
	NODE_CMD,
	NODE_PIPE,
}	t_node_type;


/**
Left-associative
EXAMPLE: cat file.txt | grep "foo" | sort >> sorted.txt

          PIPE (root)
         /          \
   PIPE              COMMAND (sort)
  /     \            argv = ["sort", NULL]
COMMAND  COMMAND     infile = NULL
  (cat)   (grep)    outfile = "sorted.txt"
argv =  ["cat", NULL] append = 1  (because of >>)
infile = "file.txt"  
outfile = NULL
append = 0

argv = ["grep", "foo", NULL]
infile = NULL
outfile = NULL
append = 0

*/
// -----------------------------------------//
//            GENERAL STRUCTS               //
// -----------------------------------------//

/**
*   @brief represents a node in the AST
*
*   allows a node to hold data for cmd_node or
*   pipe_node
*
*   @param type The type of the node (CMD, PIPE).
*   @param u_data Union holding cmd or pipe data.
*/

typedef struct s_tree_node
{
	t_node_type	type;
	union
	{
		t_cmd_data	cmd;
        // t_pipe_data pipe;
	} u_data;
} t_tree_node;

// -----------------------------------------//
//             EXECUTION STRUCTS            //
// -----------------------------------------//

/**
 *  @brief stores duplicated fd for tmp redirection
 * 
 *  -Use: to hold tmp compies of STDIN_FILENO/ STDOU_FILENO
*/
typedef struct s_fd_dup
{
	int	input_dup;
	int	output_dup;
}t_fd_dup;


// -----------------------------------------//
//                PROTOTYPES                //
// -----------------------------------------//

// --- ABSOLUTE PATH --- //

char	*find_path(char *argv);

// --- EXECUTION --- //
//Parent
int	fork_handle(t_node *node, t_node_type *type, int i_cmd, int nb_cmd);
int	setup_redir(int fd_in, int fd_out, t_fd_dup *dup);

//child
int	execute_cmd(t_node *node);

// --- ERRORS_CLEAN-UP --- //
int cleanup_fd(t_node *node, t_node_type type);

#endif