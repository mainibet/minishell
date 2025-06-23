/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: albetanc <albetanc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/21 16:32:13 by albetanc          #+#    #+#             */
/*   Updated: 2025/06/23 17:21:36 by albetanc         ###   ########.fr       */
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

//POSSIBLE FILES:
char	**dup_new_cmd(char **cmd)//YES
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

//to free dynamically allocated char ** array
static void	free_nargv(char **nargv)//chang name  YES
{
	size_t	count;

	count = 0;
	while (nargv[count])
		count++;
	free_memory(nargv, count);
	free(nargv);
}
//For external command execution
//pending to connect with envp
void	execution(char	**nargv, char **const envp)//yes
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

int	wait_child(pid_t pid, int *status)//yes
{
	if (waitpid(pid, status, 0) == -1)
	{
		perror ("Error waiting for child");
		return (-1);
	}
	return (0);
}

int	fork_handle(pid_t *pid, t_pipe_data *data, int child_num)
{
	*pid = fork();
	// if (*pid == -1)//THIS CONDITIONAL EXEUTION WILL CHANGE
	// {
	// 	perror ("Fork failed");
	// 	return (fork_error(data->fd_in, data->pipefd));
	// }
	// if (*pid == 0)
	// {
	// 	if (child_num == 1)
	// 		child1(data);
	// 	else if (child_num == 2)
	// 		child2(data);
	// 	return (1);
	// }
	return (0);
}

int	check_fork(int result, pid_t pid1, int *status)//NEED TO BE HANDLED DIFFERENT THAN 2 PIPES
{
	if (result != 0)
	{
		if (result == 1)
			return (0);
		else
		{
			if (pid1 != 0)
				wait_child(pid1, status);
			return (result);
		}
	}
	return (0);
}

static int	parent(struct s_pipe_data *data)//make it work with 1 child first and then with any
{//check how the logic changes handling external and internal commands
	pid_t	pid1;//this function needed when pips not before
	pid_t	pid2;
	int		status;
	int		result;

	result = fork_handle(&pid1, data, 1);
	if (check_fork(result, 0, &status))
		return (result);
	result = fork_handle(&pid2, data, 2);
	if (check_fork(result, pid1, &status))
		return (result);
	close_fd(data -> fd_in);
	close_fd(data -> pipefd[0]);
	close_fd(data -> pipefd[1]);
	if (wait_child(pid1, &status) == -1 || wait_child(pid2, &status) == -1)
	{
		perror ("error waiting child");
		return (-1);
	}
	return (0);
}

int	redir_input(int fd)//YES
{
	int	fd_dup;

	fd_dup = dup2(fd, STDIN_FILENO);
	if (fd_dup == -1)
	{
		perror ("Dup2 in redir_input");
		close_fd (fd);
		return (1);
	}
	close_fd(fd);
	return (0);
}

int	redir_output(int fd)//YES
{
	int	fd_dup;

	fd_dup = dup2(fd, STDOUT_FILENO);
	if (fd_dup == -1)
	{
		perror ("Dup2 in redir_output");
		close_fd(fd);
		return (1);
	}
	close_fd(fd);
	return (0);
}

int	setup_redir(int input_fd, int output_fd, t_fd_dup *dup)//MANY CHANGES NOT 2 CMD PROCESS 
{
	int	fd_in_dup;
	int	fd_out_dup;

	fd_in_dup = redir_input(input_fd);
	if (fd_in_dup < 0)
	{
		perror("Failed redir_input");
		return (-1);
	}
	fd_out_dup = redir_output(output_fd);
	if (fd_out_dup < 0)
	{
		close_fd(fd_in_dup);
		perror("Failed redirection OUTPUT");
		return (-1);
	}
	dup -> input_dup = fd_in_dup;
	dup -> output_dup = fd_out_dup;
	return (0);
}

void	child_process(t_pipe_data *data, t_initial_fd *fd, int child_num)//yes
{
	t_fd_dup	dup;
	char		**nargv;

	if (setup_redir(fd -> input_fd, fd -> output_fd, &dup) != 0)
		exit(1);
	nargv = exec_arg(data, child_num);
	if (!nargv)
	{
		perror ("nargv before execution");
		close_fd(dup.input_dup);
		close_fd(dup.output_dup);
		exit(1);
	}
	execution(nargv, data -> envp);
	perror ("Execution failed in child 1");
	free(nargv);
	close_fd(dup.input_dup);
	close_fd(dup.output_dup);
	exit(1);
}
