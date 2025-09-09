/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exp_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tpandya <tpandya@student.42berlin.de>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/09 22:35:39 by tpandya           #+#    #+#             */
/*   Updated: 2025/09/09 22:35:39 by tpandya          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*get_env_value(const char *var, char **envp_cpy)
{
	int		i;
	size_t	var_len;

	i = 0;
	var_len = ft_strlen(var);
	while (envp_cpy && envp_cpy[i])
	{
		if (ft_strncmp(envp_cpy[i], var, var_len) == 0
			&& envp_cpy[i][var_len] == '=')
			return (ft_strdup(envp_cpy[i] + var_len + 1));
		i++;
	}
	return (ft_strdup(""));
}

char	*status_exp(char c, int last_exit)
{
	char	*res;

	if (c == '?')
		res = ft_itoa(last_exit);
	else if (c == '$')
		res = ft_itoa(getpid());
	else
		res = ft_strdup("");
	return (res);
}
