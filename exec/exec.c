/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: albetanc <albetanc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/21 16:32:13 by albetanc          #+#    #+#             */
/*   Updated: 2025/06/23 16:35:59 by albetanc         ###   ########.fr       */
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

char	**dup_new_cmd(char **cmd)
{
	size_t	len;
	size_t	i;
	char	**new_arg;

	len = 0;
	i = 0;
	while (cmd[len])
		len++;
	new_arg = malloc(sizeof(char *) * (len + 1));
	if (!new_arg)
		return ((char **)malloc_error());
	while (i < len)
	{
		new_arg[i] = ft_strdup(cmd[i]);//check later where to free
		if (!new_arg[i])
		{
			free_memory(new_arg, i);
			return (NULL);
		}
		i++;
	}
	new_arg[len] = NULL;
	return (new_arg);
}

char	**exec_arg(t_pipe_data *data, int child_num)
{
	char	**new_arg;

	new_arg = new_arr_cmd(data, child_num);
	if (!new_arg)
		return (NULL);
	new_arg = dup_new_cmd(new_arg);
	if (!new_arg)
	{
		free_memory(new_arg, 0);
		free(new_arg);
		return (NULL);
	}
	return (new_arg);
}

static void	free_nargv(char **nargv)
{
	size_t	count;

	count = 0;
	while (nargv[count])
		count++;
	free_memory(nargv, count);
	free(nargv);
}
//For external command execution
void	execution(char	**nargv, char **const envp)
{
	char	*cmd_name;
	char	*cmd_path;

	cmd_name = get_only_cmd(nargv[0]);//rese get_ony_cmd
	cmd_path = find_path (cmd_name, envp);//searches the PATH environment variable
	if (!cmd_path)
	{
		perror ("command_path not found");
		free(cmd_name);
		free_nargv(nargv);
		exit(EXIT_FAILURE);
	}
	execve(cmd_path, nargv, envp);
	perror ("execve failed");
	free (cmd_name);
	free (cmd_path);
	free_nargv(nargv);
	exit (EXIT_FAILURE);
}


