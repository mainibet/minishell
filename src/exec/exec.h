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

// -----------------------------------------//
//               TREE NODES                 //
// -----------------------------------------//
typedef enum e_node_type//CREATED FOR TESTING EXEC
{
	NODE_CMD,
	NODE_PIPE,
  NODE_REDIR_IN,
  NODE_REDIR_OUT,
  NODE_REDIR_APPEND,
  NODE_HEREDOC,
  NODE_FILE,
}	t_node_type;

// -----------------------------------------//
//                 STRUCTS                  //
// -----------------------------------------//
typedef struct s_node//CREATED FOR TESTING EXEC
{
	  t_node_type type;
    char **argv;
    char *filename;
    struct s_node *left;//to PIPE: left
    struct s_node *right;//to PIPE right
}	t_node;

/*
EXAMPLE: cat < input.txt >> output.log

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