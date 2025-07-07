/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: albetanc <albetanc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/21 11:02:07 by albetanc          #+#    #+#             */
/*   Updated: 2025/07/07 07:46:08 by albetanc         ###   ########.fr       */
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
//here later we can include bonus
typedef enum e_node_type//CREATED FOR TESTING EXEC
{
	NODE_CMD,
	NODE_PIPE,
}	t_node_type;

// // ---------------------------------------//
//           MAIN UNION STRUCTS             //
// -----------------------------------------//
typedef struct s_tree_node
{
	t_node_type	type; //type of the node (command, pipe)
	union
	{
		t_cmd_data	cmd; //content of the node (argv, redir, etc.)
		t_pipe_data		pipe;
	} u_data;
}	t_tree_node;

// -----------------------------------------//
//         REDIRECTION STRUCTS              //
// -----------------------------------------//
typedef enum e_redir_type
{
	REDIR_IN,
	REDIR_OUT,
	REDIR_APPEND,
	HEREDOC,
}	t_redir_type;

typedef struct s_redir
{
	t_redir_type	type;//in,out, append,heredoc
	char			*filename;//file or delimiter
} t_redir;

// -----------------------------------------//
//               COMMAND TYPE               //
// -----------------------------------------//
typedef enum e_cmdtype
{
	EXECUTABLE,
	BUILTIN,
	MAX_CMDTYPE//what is this for?
}	t_cmdtype;

// -----------------------------------------//
//                 NODE CONTENT             //
// -----------------------------------------//

// --- possible future version --- //
// typedef struct s_cmd_data
// {
// 	char			**argv; //command and arguments
// 	t_list			*redir; //linkedlist of redirections
// 	int				redir_count; //number of redirections
// 	enum e_cmdtype	cmd_type;//cmd type
// } t_cmd_data;

// --- initial version --- //
typedef struct s_cmd_data
{
        char	**argv;//this will be in reallity: t_token *tokens;
        char	**env;
		int		fd_in;
		int		fd_out;
		enum	e_cmdtype;
        // int     io[2];//fd in and fd out
        // int     fdother[MAX_OPEN_FILES]; // open fds to close only in child process, -1 terminated
        //include the struct of pipes
}       t_cmd_data;

// -----------------------------------------//
//             EXECUTION STRUCTS            //
// -----------------------------------------//


typedef struct s_pipe_data
{
	t_tree_node	*left; //recursive dependency
	t_tree_node	*right; //recursive dependency
	int			input_fd;
	int			output_fd;
    //include the struct of cmd
}	t_pipe_data;

typedef struct s_fd_dup
{
	int	input_dup;
	int	output_dup;
}t_fd_dup;



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
//                PROTOTYPES                //
// -----------------------------------------//

// --- ABSOLUTE PATH --- //

char	*find_path(char *argv);

// --- REDIRECTIONS --- //
int	setup_redir(int fd_in, int fd_out, t_fd_dup *fd);

// --- EXECUTION --- //
int	fork_handle(t_node *node, t_node_type *type, int i_cmd, int nb_cmd);
void	child_first(t_node *node, t_node_type *type);
void    child_middle(t_node *node, t_node_type *type);
void	child_last(t_node *node, t_node_type *type);

// --- ERRORS_CLEAN-UP --- //
int cleanup_fd(t_node *node, t_node_type *type);

//centralized error messages

#endif