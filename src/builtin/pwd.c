/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: albetanc <albetanc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/11 09:27:29 by albetanc          #+#    #+#             */
/*   Updated: 2025/08/11 10:01:41 by albetanc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "exec.h"//TMP FOR TESTING

static char	*find_env_value(char **envp, const char *key)
{
	int	i;
	int	len;

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

//cwd: current working directory
//Original pwd: Print the full 
//filename of the current working directory.
int	my_pwd(t_program *program, t_node *node)
{
	char	*cwd_path;

	(void) node;
	cwd_path = find_env_value(program->envp_cpy, "PWD");
	if (cwd_path)
	{
		printf("%s\n", cwd_path);
		return (0);
	}
	else
	{
		fprintf(stderr, BLUE "pwd: current directory not found\n" RESET);
		return (1);
	}
}
