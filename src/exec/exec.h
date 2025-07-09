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
	T_NODE_COMMAND,
	T_NODE_PIPE,
}	t_node_type;

// -----------------------------------------//
//                 STRUCTS                  //
// -----------------------------------------//
typedef struct s_node//CREATED FOR TESTING EXEC
{
	char	**argv;// comand + args
	char	*infile;//input fd for redirections, includes the "<"
	char	*outfile;//output fd for redirections, includes ">" ">>"
	int		append_flag;//to know if there is or not ">>" > can be 0 and >> can be 1
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

char	*find_path(char *argv, char **envp);


#endif