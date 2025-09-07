/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: albetanc <albetanc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/21 11:02:07 by albetanc          #+#    #+#             */
/*   Updated: 2025/08/26 15:17:49 by albetanc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXEC_H
# define EXEC_H

# include "minishell.h"
# include <stdbool.h> //bool

// -----------------------------------------//
//                PROTOTYPES                //
// -----------------------------------------//

// int		process_node(t_program *program);

// --- ABSOLUTE PATH --- //
char	**get_path(void);
char	*create_path(char *dir, char *argv);
char	*check_path(char **dir, char *file_path, int i);
char 	*free_prev_dir(char **dir, int i);
int	is_executable(char *path);
char	*find_in_path_dirs(char **dir, char *argv);
char	*find_path(char *argv);

// --- EXECUTION --- //
//Parent cmd node
// int		fork_handle(pid_t *token_headid, t_node *node, int i_cmd, int nb_cmd);
// int		setup_redir(int fd_in, int fd_out, t_fd_dup *dup);
int		execution(t_program *program, t_node *node, bool is_pipe_child);
int		handle_cmd_exec(t_program *program, t_node *node, bool is_pipe_child);
int		exec_cmd_nopipe(t_program *token_promgra, t_node *node);

//child
void	child_process(t_program *program, t_node *node);

//pipelines
void	exec_cmd_inchild(t_node *node);
int		execute_pipeline(t_program *program, t_node *node);
int		wait_children(pid_t left_pid, pid_t right_pid, int *right_status);
void	set_final_fds(t_cmd_data *cmd);

// Helper functions

int	handle_operator(t_program *program, t_node *node, bool is_pipe_child);
void	set_pipe_input(t_cmd_data *cmd);
void	set_input_redir(t_cmd_data *cmd);
void	set_pipe_output(t_cmd_data *cmd);
void	set_output_redir(t_cmd_data *cmd);
int	handle_builtin_no_pipe(t_program *program, t_node *node, t_cmd_data *cmd);
void	child_exec_flow(t_program *program, t_node *node, t_cmd_data *cmd);
int	external_no_pipe(t_program *program, t_node *node, t_cmd_data *cmd);

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
