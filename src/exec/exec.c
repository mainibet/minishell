/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: albetanc <albetanc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/21 16:32:13 by albetanc          #+#    #+#             */
/*   Updated: 2025/07/09 07:53:18 by albetanc         ###   ########.fr       */
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

/**
 * @brief Executes a single command in the current process.
 * 
 * 1. Finds cmd_path
 * 2. Handles path not found
 * 3. Executes external cmd
 * 4. Handles execution (execve) failure
 * 
 * @note This version will exit shell when cmd executes 
 * ('cause there are no forks)
 */

void	execution (t_node *root, char **envp)
{
	char	*cmd_path;

	cmd_path = find_path (root->argv[0], envp);
	if (!cmd_path)
	cmd_path = find_path (root->argv[0], envp);
	if (!cmd_path)
	{
		perror ("command_path not found");
		perror ("command_path not found");
		exit(EXIT_FAILURE);
	}
	execve(cmd_path, root->argv, envp);
	perror ("execve failed");
	free (cmd_path);
	exit (EXIT_FAILURE);
	free (cmd_path);
	exit (EXIT_FAILURE);
}