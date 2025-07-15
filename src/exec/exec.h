/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: albetanc <albetanc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/21 11:02:07 by albetanc          #+#    #+#             */
/*   Updated: 2025/07/15 17:36:23 by albetanc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXEC_H
# define EXEC_H

# include "minishell.h"

// --- FORWARD DECLARATION --- //
typedef struct s_node t_node;

// -----------------------------------------//
//                 STRUCTS                  //
// -----------------------------------------//

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
	char		**argv;
	char		**env;
	int			fd_in;
	int			fd_out;
	t_cmdtype	cmd_type;
}	t_cmd_data;

typedef struct s_pipe_data
{
	t_node	*left;
	t_node	*right;
}	t_pipe_data;


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
int		fork_handle(pid_t *pid, t_node *node, int i_cmd, int nb_cmd);
int		setup_redir(int fd_in, int fd_out, t_fd_dup *dup);
int		wait_one_child(pid_t pid, int *status);
int		execute_cmd(t_node *node);

//child
void	child_process(t_node *node);
void	exec_external_cmd(t_node *node);

// --- ERRORS_CLEAN-UP --- //
// int		cleanup_fd(t_node *node, t_node_type type);
int		cleanup_fd(t_node *node, int node_type);
int		cleanup_cmd_node(t_node *node);

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

#endif