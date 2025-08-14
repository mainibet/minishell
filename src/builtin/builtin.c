/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: albetanc <albetanc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/31 18:10:28 by albetanc          #+#    #+#             */
/*   Updated: 2025/08/11 14:15:33 by albetanc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "builtin.h"

int	is_builtin(const char *cmd_name)
{
	if (!cmd_name)
		return (BUILTIN_NONE); // No es un builtin si no hay nombre

	if (ft_strcmp(cmd_name, "echo") == 0)
		return (BUILTIN_ECHO);
	// if (ft_strcmp(cmd_name, "cd") == 0)
	// 	return (BUILTIN_CD);
	if (ft_strcmp(cmd_name, "pwd") == 0)
		return (BUILTIN_PWD);
	if (ft_strcmp(cmd_name, "export") == 0)
		return (BUILTIN_EXPORT);
	// if (ft_strcmp(cmd_name, "unset") == 0)
	// 	return (BUILTIN_UNSET);
	if (ft_strcmp(cmd_name, "env") == 0)
		return (BUILTIN_ENV);
	if (ft_strcmp(cmd_name, "exit") == 0)
		return (BUILTIN_EXIT);
	return (BUILTIN_NONE);
}

// int	execute_builtin(t_node *node)
int	execute_builtin(t_program *program, t_node *node)
{
	t_cmd_data	*cmd;
	t_builtin_type	e_builtin_type;

	cmd = &node->u_data.cmd;
	e_builtin_type = is_builtin(cmd->argv[0]);

	if (e_builtin_type == BUILTIN_ECHO)
		return (my_echo(node));
	if (e_builtin_type == BUILTIN_ENV)
		return (my_env(program, node));//implement program->envp-cpy
	// else if (e_builtin_type == BUILTIN_CD)
	// 	return (my_cd(node)); 
	else if (e_builtin_type == BUILTIN_PWD)
		return (my_pwd(program, node));
	else if (e_builtin_type == BUILTIN_EXPORT)
		return (my_export(program, node));//implement program->envp-cpy
	else if (e_builtin_type == BUILTIN_EXIT)
		return (my_exit(program, node));
	fprintf(stderr, BOLD RED "Error: unknown builtin type for execution\n" RESET);//check if needed
	return (1);
}
