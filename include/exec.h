/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: albetanc <albetanc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/21 11:02:07 by albetanc          #+#    #+#             */
/*   Updated: 2025/07/31 17:55:23 by albetanc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXEC_H
# define EXEC_H

# include "minishell.h"

// -----------------------------------------//
//                 STRUCTS                  //
// -----------------------------------------//

/**
 *  @brief stores duplicated fd for tmp redirection
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
//Parent cmd node
int		fork_handle(pid_t *pid, t_node *node, int i_cmd, int nb_cmd);
int		setup_redir(int fd_in, int fd_out, t_fd_dup *dup);
int		wait_one_child(pid_t pid, int *status);
int		execution(t_node *cmd);
int		execute_cmd(t_node *node);

//child
void	child_process(t_node *node);
void	exec_external_cmd(t_node *node);

//pipelines
int		execute_pipeline(t_node *node);
int		wait_children(pid_t left_pid, pid_t right_pid, int *right_status);

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