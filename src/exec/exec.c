/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: albetanc <albetanc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/21 16:32:13 by albetanc          #+#    #+#             */
/*   Updated: 2025/08/26 09:53:57 by albetanc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

//Executes a cmd with execve
//doesn't fork
void	exec_cmd_inchild(t_node *node)
{
	char	*cmd_path;

	cmd_path = find_path (node->u_data.cmd.argv[0]);
	if (!cmd_path)
	{
		perror (BOLD RED "command_path not found" RESET);
		exit(EXIT_FAILURE);
	}
	execve(cmd_path, node->u_data.cmd.argv, node->u_data.cmd.env);
	perror("Error: execve failed");
	free(cmd_path);
	exit(EXIT_FAILURE);
}

static int	handle_operator(t_program *program, t_node *node, bool is_pipe_child)//NEW
{
	int	left_status;
	int	right_status;

	if (node->u_data.op.type == PIPE)
		return (execute_pipeline(program, node));
	else if (node->u_data.op.type == AND)
	{
		left_status = execution(program, node->u_data.op.left, true);
		if (left_status == 0)
			return (execution(program, node->u_data.op.right, true));
		return (left_status);
	}
	// else if (node->u_data.op.type == OR)
	// {
	// 	left_status = execution(program, node->u_data.op.left);
	// 	if (left_status != 0)
	// 		return (execution(program, node->u_data.op.right));
	// 	return (left_status);
	// }
	// else if (node->u_data.op.type == SEMICOLON)
	// {
	// 	execution(program, node->u_data.op.left);
	// 	return (execution(program, node->u_data.op.right));
	// }
	fprintf(stderr, BOLD RED "Error: unknow type operand for execution\n" RESET);
	return (1);
}

int	is_operator_str(const char *str)
{
	if (!str)
		return (0);
	if (ft_strcmp(str, "|") == 0
		|| ft_strcmp(str, "&&") == 0
		|| ft_strcmp(str, "||") == 0
		|| ft_strcmp(str, ";") == 0)
		return (1);
	return (0);
}

// in child before executing cmd
//connect pipes if there is no redir
void	set_final_fds(t_cmd_data *cmd)
{
	if (cmd->fd_in == STDIN_FILENO && cmd->pipefd[0] > 2)
		cmd->fd_in = cmd->pipefd[0];
	if (cmd->fd_out == STDOUT_FILENO && cmd->pipefd[1] > 2)
		cmd->fd_out = cmd->pipefd[1];
	if (cmd->fd_in != STDIN_FILENO)
	{
		redir_in(cmd->fd_in);
		close_fd(&cmd->fd_in);
	}
	if (cmd->fd_out != STDOUT_FILENO)
	{
		redir_out(cmd->fd_out);
		close_fd(&cmd->fd_out);
	}
}

// decides cmd execution
int	handle_cmd_exec(t_program *program, t_node *node, bool is_pipe_child)
{
	int			status;
	char		*cmd_name;
	t_cmd_data	*cmd;

	if (!node || !node->u_data.cmd.argv)
		return (1);
	cmd_name = node->u_data.cmd.argv[0];
	if (is_operator_str(cmd_name))
	{
		fprintf(stderr, RED BOLD
			"Syntax error near unexpected token `%s`\n" RESET, cmd_name);
		return (1);//syntax error can be 2?
	}
	if (process_redir(&node->u_data.cmd, program) != 0)//NEEDED
		exit(EXIT_FAILURE);//NEEDED check if return (1) instead
	if (is_pipe_child)
	{
		set_final_fds(cmd);
		if (is_builtin(cmd_name))
		{
			status = execute_builtin(program, node, true);
			exit (status);
		}
		else
		{
			exec_cmd_nopipe(program, node);
			restore_std(program);
			exit (1);
		}
	}
	else
	{
		if (is_builtin(cmd_name))
		{
			cmd = &node->u_data.cmd;
			if (cmd->redir) 
			{
				if (process_redir(cmd, program) == 0)
					setup_redir(cmd);
				cmd->fd_out = STDOUT_FILENO;
			}
			status = execute_builtin(program, node, false);
			if (cmd->redir)
				restore_std(program);
			program->last_exit_status = status;
			return (status);
		}
		else
			return (exec_cmd_nopipe(program, node));
	}
}

int	execution(t_program *program, t_node *node, bool is_pipe_child)
{
	int	status;

	fprintf(stderr, MAGENTA BOLD "About to dispatch a node execution\n" RESET);//test
	if (!node)
	{
		program->last_exit_status = 0;
		fprintf(stderr, MAGENTA BOLD "Last cmd updated: %d\n" RESET, program->last_exit_status);//test
		return (0);
	}
	if (node->type == COMMAND) 
	{
		status = handle_cmd_exec(program, node, is_pipe_child);
		fprintf(stderr, MAGENTA BOLD "Will be a cmd\n" RESET);//test
	}
	else if (node->type == OPERATOR)
	{
		status = handle_operator(program, node, is_pipe_child);
		fprintf(stderr, MAGENTA BOLD "Will be a oprator\n" RESET);//test
	}
	else
	{
		fprintf(stderr, BOLD RED "Error: unknow type node for execution\n" RESET);
		status = 1;
	}
	program->last_exit_status = status;
	return (status);
}

