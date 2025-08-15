

//INCLUDE 42 HEADER

# include "minishell.h"

char	*find_env_value(char **envp, const char *key)
{
	int	i;
	int	len;

	fprintf(stderr, MAGENTA BOLD "MY PWD or CD is about to be run\n" RESET);
	if (!envp || !key)
		return (NULL);
	len = ft_strlen(key);
	i = 0;
	while (envp[i])
	{
		if (ft_strncmp(envp[i], key, len) == 0 && envp[i][len] == '=')
			return (&envp[i][len + 1]);
		i++;
	}
	return (NULL);
}
