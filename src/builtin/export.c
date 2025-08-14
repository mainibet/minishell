/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: albetanc <albetanc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/11 11:12:31 by albetanc          #+#    #+#             */
/*   Updated: 2025/08/11 13:59:45 by albetanc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "exec.h"//TMP FOR TESTING

/*
*	@brief Prints all currently exported environment variables
*	in the declare -x format
*	@returns 0 on success and 1 on error
*/
// int	my_export(char **envp)
int	my_export(t_program *program, t_node *node)
{
	int		i;
	char	**args;

	args = node->u_data.cmd.argv;
	if (node->u_data.cmd.argv == 1)
	{
		if (!program->envp_cpy)
		{
			fprintf(stderr, BLUE "export: no environment variable found\n" RESET);
			return (1);
		}
		i = 0;
		while (program->envp_cpy[i])
		{
			printf(GREEN "declare -x %s\n" RESET, program->envp_cpy[i]);
			i++;
		}
	}
	// else
	//TODOL export VAR=value and others args and update program envp cpyif needed
	return (0);
}

