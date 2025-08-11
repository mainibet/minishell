/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: albetanc <albetanc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/11 11:12:31 by albetanc          #+#    #+#             */
/*   Updated: 2025/08/11 11:38:17 by albetanc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "exec.h"//TMP FOR TESTING

int	my_export(char **envp)
{
	int	i;

	if (!envp)
	{
		fprintf(stderr, BLUE "export: no environment variable found\n" RESET);
		return (1);
	}
	i = 0;
	while (envp[i])
	{
		printf(GREEN "declare -x %s\n" RESET, envp[i]);
		i++;
	}
	return (0);
}

