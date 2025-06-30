

int	close_fd(int fd)
{
	if (close(fd) == -1)
	{
		perror ("error closing fd");
		return (1);
	}
	return (0);
}

void	free_memory(char **narg, int j)
{
	while (j > 0)
	{
		free (narg[j - 1]);
		j--;
	}
	free (narg);
}

int	fork_error(int fd_in, int *pipefd)
{
	close_fd(fd_in);
	close_fd(pipefd[0]);
	close_fd(pipefd[1]);
	return (-1);
}

char	*malloc_error(void)
{
	perror("Malloc error");
	return (NULL);
}

# include <fcntl.h> 
# include <unistd.h>

// Returns an array of directory paths from the PATH environment variable.
/*
    Pipex version
*/
// static char	**get_path(char **envp)
// {
// 	char	*path;
// 	char	**dir;

// 	path = get_path_env(envp);
// 	if (!path)
// 	{
// 		perror("PATH env not found");
// 		return (NULL);
// 	}
// 	dir = ft_split(path, ':');
// 	if (!dir)
// 	{
// 		free(path);
// 		return (NULL);
// 	}
// 	return (dir);
// }

/*
    Proposal minishell
*/
static char	**get_path(void)
{
	char	*path;
	char	**dir;

	path = getenv("PATH"); // new function allowed in minishell
	if (!path)
	{
		perror("PATH not found");
		return (NULL);
	}
	dir = ft_split(path, ':');
	return (dir);
}

// Joins a directory and a filename to create a full file path.
static char	*create_path(char *dir, char *argv)
{
	char	*each_path;
	char	*file_path;

	each_path = ft_strjoin(dir, "/");
	if (!each_path)
	{
		free (dir);
		return (NULL);
	}
	file_path = ft_strjoin(each_path, argv);
	free (each_path);
	if (!file_path)
	{
		free(dir);
		return (NULL);
	}
	return (file_path);
}

// Checks if the file at file_path exists; frees memory and returns the path if found.
static char	*check_path(char **dir, char *file_path, int i)
{
	if (access (file_path, F_OK) == 0)
	{
		while (dir[i])
		{
			free(dir[i]);
			i++;
		}
		free (dir);
		return (file_path);
	}
	return (NULL);
}

// Frees all directory strings from index i onwards and returns NULL.
static char	*free_prev_dir(char **dir, int i)
{
	while (dir[i])
	{
		free(dir[i]);
		i++;
	}
	free(dir);
	return (NULL);
}

// Finds the full path of a command by searching directories in PATH.
char	*find_path(char *argv, char **envp)
{
	char	*file_path;
	char	**dir;
	char	*final_path;
	int		i;

	dir = get_path(envp);
	if (!dir)
		return (NULL);
	i = 0;
	while (dir[i])
	{
		file_path = create_path(dir[i], argv);
		if (!file_path)
			free_prev_dir(dir, i);
		final_path = check_path(dir, file_path, i);
		if (final_path)
			return (final_path);
		free(file_path);
		free (dir[i]);
		i++;
	}
	free(dir);
	return (NULL);
}

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
		new_arg[i] = ft_strdup(cmd[i]);//check where to free later
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

//Initially I received a string with the cmd, flags and args
//With the tree I tree node and envp
// void	execution(char	**nargv, char **const envp)
//V0 just executes single cmd
//WHat tokenzation did: char *argv[] = {"ls", "-l", "/home", NULL};
//argv[0] is command name
void    execution (t_node *root, char **envp)
{
	// char	*cmd_name;
	char	*cmd_path;

	// cmd_name = get_only_cmd(nargv[0]);This was needed to find the path. NOT NEEDED IN MINISHELL THEN
	cmd_path = find_path (root->argv[0], envp);//I will received this from parsing
	if (!cmd_path)//this needs to be done in the parsing
	{
		perror ("command_path not found");//nclude general celan-up and check were to call t for free root_node
		exit(EXIT_FAILURE);
	}
	execve(cmd_path, root->argv, envp);
	perror ("execve failed");
	// free (cmd_name);
	free (cmd_path);//freed here 'cause was created here and used here
	// free (root->argv);check later where to free
	exit (EXIT_FAILURE);//later a general celan-up to free all nodes
}