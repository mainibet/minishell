/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: albetanc <albetanc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/21 16:32:13 by albetanc          #+#    #+#             */
/*   Updated: 2025/06/30 17:16:58 by albetanc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

//---------------------------------------//
//V0:Execute single commands directly,   //
//   from readline                       //
//                                       //
//V1:supports basic parsing              //
//                                       //
//V2:support parsing refinement          //
//                                       //
//V3:execution with pipes & child pcs    //
//                                       //
//V4: support all built-ins              //
//                                       //
//V5: support heredoc & redirections     //
//---------------------------------------//

// Read a full line as raw input.
// Use fork() and execvp() to execute the input line as-is.
// Wait for the child process (waitpid).
// Exit command ends the shell.
// Should check for builtins commands if not, look for external commands ($PATH)

// need to duplicate the arguments (char **argv) before passing them to the child process 
//duplicating the arguments gives the child process its own isolated and safe copy of the command-line arguments. 
//  This prevents unintended side effects and memory issues between the parent and child processes, 
//  particularly crucial when execve() is about to replace the child's entire memory space.

//VALORACION TMP PARA EMPEZAR
//  For each external command in that list, you will:
// Fork a new child process.
// Set up redirections (using adapted setup_redir and redir_input/output).
// Execute the command using execve (similar to your execution function).
// In the parent, wait for that child (using wait_child). This generic, looped approach is what allows Minishell to handle N commands.

//Initially I received a string with the cmd, flags and args
//With the tree I tree node and envp
// void	execution(char	**nargv, char **const envp)
//V0 just executes single cmd
//WHat tokenzation did: char *argv[] = {"ls", "-l", "/home", NULL};
//argv[0] is command name

void	execution (t_node *root, char **envp)
{
	char	*cmd_path;

	cmd_path = find_path (root->argv[0], envp);//I will received this from parsing
	if (!cmd_path)//this needs to be done in the parsing
	{
		perror ("command_path not found");//TODO: include general celan-up and check were to call t for free root_node
		exit(EXIT_FAILURE);
	}
	execve(cmd_path, root->argv, envp);
	perror ("execve failed");
	free (cmd_path);//freed here 'cause was created here and used here
	//TODO: include general celan-up and check were to call t for free root_node
	exit (EXIT_FAILURE);//later in the program a general celan-up to free all nodes
}