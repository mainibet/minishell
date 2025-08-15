//42 HEADER

# include "minishell.h"

static char	*create_env_entry(const char *key, const char *value)
{
	char	*tmp_str;
	char	*new_entry;

	tmp_str = ft_strjoin(key, "=");//check where to free
	if (!tmp_str)
		return NULL;
	new_entry = ft_strjoin(tmp_str, value);//check where to free
	free(tmp_str);
	return new_entry;
}

static void add_env_var(t_program *program, const char *key, const char *value)
{
	char	**envp;
	int		count;
	int		i;

	count = 0;
	envp = program->envp_cpy;
	while (envp[count])
		count++;
	char **new_envp = malloc(sizeof(char *) * (count + 2));
	if (!new_envp)
		return;
	i = 0;
	while (envp[i])
	{
		new_envp[i] = envp[i];
		i++;
	}
	new_envp[count] = create_env_entry(key, value);
	if (!new_envp[count])
	{
		free(new_envp);
		return;
	}
	new_envp[count + 1] = NULL;
	free(program->envp_cpy);
	program->envp_cpy = new_envp;
}

void update_env_var(t_program *program, const char *key, const char *value)
{
	char	**envp;
	int		i;
	int		len;

	if (!program || !program->envp_cpy || !key || !value)
		return;
	envp = program->envp_cpy;
	len = ft_strlen(key);
	i = 0;
	while (envp[i])
	{
		if (ft_strncmp(envp[i], key, len) == 0 && envp[i][len] == '=')
		{
			char *new_entry = create_env_entry(key, value);
			if (!new_entry)
				return;
			free(envp[i]);
			envp[i] = new_entry;
			return;
		}
		i++;
	}
	add_env_var(program, key, value);
}
