/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: albetanc <albetanc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/31 18:10:28 by albetanc          #+#    #+#             */
/*   Updated: 2025/08/22 16:46:42 by albetanc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	is_builtin(const char *cmd_name)
{
	if (!cmd_name)
		return (BUILTIN_NONE);
	if (ft_strcmp(cmd_name, "echo") == 0)
		return (BUILTIN_ECHO);
	if (ft_strcmp(cmd_name, "cd") == 0)
		return (BUILTIN_CD);
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

void	apply_redir(t_cmd_data *cmd)
{
	if (cmd->fd_in != STDIN_FILENO)
	{
		dup2(cmd->fd_in, STDIN_FILENO);
		close_fd(&cmd->fd_in);
	}
	if (cmd->fd_out != STDOUT_FILENO)
	{
		dup2(cmd->fd_out, STDOUT_FILENO);
		close_fd(&cmd->fd_out);
	}
}

static void	restore_std(t_program *program)
{
	dup2(program->fd_in_orig, STDIN_FILENO);
	dup2(program->fd_out_orig, STDOUT_FILENO);
}

int	execute_builtin(t_program *program, t_node *node, bool is_pipe_child)
{
	t_cmd_data		*cmd;
	t_builtin_type	e_builtin_type;
	int				status;

	apply_redir(cmd);
	cmd = &node->u_data.cmd;
	status = 0;
	e_builtin_type = is_builtin(cmd->argv[0]);
	if (e_builtin_type == BUILTIN_ECHO)
		status = my_echo(program, node);
	else if (e_builtin_type == BUILTIN_ENV)
		status = my_env(program, node);
	else if (e_builtin_type == BUILTIN_CD)
		status = my_cd(program, node); 
	else if (e_builtin_type == BUILTIN_PWD)
		status = my_pwd(program, node);
	else if (e_builtin_type == BUILTIN_EXPORT)
		status = my_export(program, node);
	else if (e_builtin_type == BUILTIN_EXIT)
		my_exit(program, node);
	else
	{
		fprintf(stderr, BOLD RED "Error: unknown builtin\n" RESET);
		status = 1;
	}
	if (!is_pipe_child)
		restore_std(program);
	return (status);
}
