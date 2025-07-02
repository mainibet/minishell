/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: albetanc <albetanc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/21 11:02:07 by albetanc          #+#    #+#             */
/*   Updated: 2025/06/30 17:16:13 by albetanc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXEC_H
# define EXEC_H

// --- STANDARD HEADERS ---
#include <readline/readline.h>
#include <readline/history.h>

// ---FORWARD DECLARATIONS--- //
typedef struct s_tree_node t_node;

// -----------------------------------------//
//           TREE NODES TYPES               //
// -----------------------------------------//
typedef enum e_node_type//CREATED FOR TESTING EXEC
{
	NODE_CMD,
	NODE_PIPE,
}	t_node_type;

// -----------------------------------------//
//         REDIRECTION STRUCTS              //
// -----------------------------------------//
typedef enum e_redir_type
{
  REDIR_IN,
  REDIR_OUT,
  REDIR_APPEND,
  HEREDOC,
} t_redir_type;

typedef struct s_redir
{
  t_redir_type  type;//in,out, append,heredoc
  char          *filename;//file or delimiter
} t_redir;

// -----------------------------------------//
//                 NODE CONTENT             //
// -----------------------------------------//

typedef struct s_cmd_content
{
  char          **argv; //command and arguments
  t_list       *redir; //linkedlist of redirections
  int           redir_count; //number of redirections
} t_cmd_content;

typedef struct s_pipe_data
{
  t_tree_node *left; //recursive dependency
  t_tree_node *right; //recursive dependency
} t_pipe_data;

// // ---------------------------------------//
//           MAIN UNION STRUCTS             //
// -----------------------------------------//
typedef struct s_tree_node
{
  t_node_type		type; //type of the node (command, pipe)
  union
  {
    t_cmd_content	content; //content of the node (argv, redir, etc.)
    t_pipe_data;
}data;
} t_tree_node;

/*EXAMPLE: cat < input.txt >> output.log

node->argv = ["cat", NULL];
node->infile = strdup("input.txt");
node->outfile = strdup("output.log");
node->append_flag = 1;
node->left = NULL;
node->right = NULL;
*/

/*
EXAMPLE: grep "foo" < input.txt | sort > output.txt
        
        PIPE (root)
       /          \
[COMMAND grep]   [COMMAND sort]
argv = ["grep", "foo", NULL]
infile = "input.txt"
outfile = NULL
append = 0

argv = ["sort", NULL]
infile = NULL
outfile = "output.txt"
append = 0

*/

/*

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
//                PROTOTYPES                //
// -----------------------------------------//

// --- ABSOLUTE PATH --- //

char	*find_path(char *argv);


// --- ERRORS_CLEAN-UP --- //
//centralized error messages

#endif