
//INCLUDE 42 HEADER

# include "minishell.h"

static int	handle_env_path(t_program *program, char *key, char **dest_path)
{
	char	*tmp_path;

	tmp_path = find_env_value(program->envp_cpy, key);
	if (!tmp_path)
	{
		fprintf(stderr, "cd: %s not set\n", key);
		return (1);
	}
	*dest_path = ft_strdup(tmp_path);//check where to free
	if (!*dest_path)
	{
		fprintf(stderr, RED BOLD "cd: memory error in handl env path\n" RESET);
		return (1);
	}
	return (0);
}

int	get_dest_path(t_program *program, t_node *node, char **dest_path)
{
	if (!node->u_data.cmd.argv[1] || ft_strcmp(node->u_data.cmd.argv[1], "~") == 0)
		return (handle_env_path(program, "HOME", dest_path));
	else if (ft_strcmp(node->u_data.cmd.argv[1], "-") == 0)
		return (handle_env_path(program, "OLDPWD", dest_path));
	else
	{
		*dest_path = ft_strdup(node->u_data.cmd.argv[1]);
		if (!*dest_path)
		{
			fprintf(stderr, "cd: memory allocation error\n");
			return (1);
		}
		return (0);
	}
}

int	change_dir(char *dest_path)
{
	if (chdir(dest_path) == -1)
	{
		perror("Builtin error: cd");
		return (1);
	}
	return (0);
}

void	update_paths(t_program *program, char *old_pwd, char *new_path)
{
	update_env_var(program, "OLDPWD", old_pwd);
	update_env_var(program, "PWD", new_path);
}

static void	free_old_dest(char *old_pwd, char *dest_path)
{
	free(old_pwd);
	free(dest_path);
}

int	my_cd(t_program *program, t_node *node)
{
	char	*old_pwd;
	char	*new_cwd;
	char	*dest_path;
	int		status;

	status = get_dest_path(program, node, &dest_path);
	if (status != 0)
		return (status);
	old_pwd = getcwd(NULL, 0);
	if (!old_pwd)
	{
		fprintf(stderr, RED BOLD "cd: getcwd error to get path\n" RESET);
		free(dest_path);
		return (1);
	}
	status = change_dir(dest_path);
	if (status != 0)
	{
		free_old_dest(old_pwd, dest_path);
		return (status);
	}
	new_cwd = getcwd(NULL, 0);
	if (!new_cwd)
	{
		fprintf(stderr, RED BOLD "cd: getcwd error when gettin new_cwd\n" RESET);
		free_old_dest(old_pwd, dest_path);
		return (1);
	}
	update_paths(program, old_pwd, new_cwd);
	free_old_dest(old_pwd, dest_path);
	free(new_cwd);
	char *new_pwd = find_env_value(program->envp_cpy, "PWD");//test
	fprintf(stderr, MAGENTA BOLD "DEBUG - new pwd is: %s\n" RESET, new_pwd);
	return (0);
}
