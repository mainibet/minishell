
//PENDING 42 HEADER

#include "minishell.h"

static char	**ft_dup_env(char **envp)
{
	int i;
	char **envp_cpy;

	i = 0;
	while (envp[i])
		i++;
	envp_cpy = malloc(sizeof(char *) * (i + 1));
	if (!envp_cpy)
		return (NULL);
	i = 0;
	while (envp[i])
	{
		envp_cpy[i] = ft_strdup(envp[i]);//check were to free
		if (!envp_cpy[i])
		{
			envp_cpy[i] = NULL;
			free_array(envp_cpy);
			return (NULL);
		}
		i++;
	}
	envp_cpy[i] = NULL;
	return (envp_cpy);
}

void	init_program(t_program *program, char **envp)
{
	program->line = NULL;
	program->root = NULL;
	program->token_list = NULL;
	program->envp = envp;
	program->envp_cpy = ft_dup_env(envp);
	program->last_exit_status = 0;
	if (!program->envp_cpy)
		malloc_error();
}
