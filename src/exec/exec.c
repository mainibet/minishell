/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: albetanc <albetanc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/21 16:32:13 by albetanc          #+#    #+#             */
/*   Updated: 2025/07/09 07:23:56 by albetanc         ###   ########.fr       */
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