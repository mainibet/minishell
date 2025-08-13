/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prexec.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: albetanc <albetanc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/16 08:41:11 by albetanc          #+#    #+#             */
/*   Updated: 2025/08/06 11:49:27 by albetanc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "prexec.h"
#include "builtin.h"

static int	count_tokens(t_token *token)
{
	int	count;

	count = 0;
	while (token)
	{
		count++;
		token = token->next;
	}
	return (count);
}

void	free_partial_arr(char **arr, int allocated_i)
{
	if (!arr)//check if needed
		return ;
	while (allocated_i > 0)
	{
		free (arr[allocated_i - 1]);
		allocated_i--;
	}
	free (arr);
}

static int copy_token_strings(char **argv, t_token *token)
{
	t_token	*current_token;
	int		i;

	current_token = token;
	i = 0;
	while (current_token && current_token->type != OPERATOR)
	{
		argv[i] = ft_strdup(current_token->txt);
		if (!argv[i])
		{
			perror ("Failed ft_strdup token to argv");//check
			free_partial_arr(argv, i);
			return (1);
		}
		current_token = current_token->next;
		i++;
	}
	argv[i] = NULL;
	return (0);
}

char	**token_to_argv(t_token *token)
{
	char	**argv;
	t_token	*current_token;
	int		nb_token;
	// int		i;

	nb_token = 0;
	current_token = token;
	while (current_token && current_token->type != OPERATOR)
	{
		nb_token++;
		current_token = current_token->next;
	}
	argv = malloc(sizeof(char *) * (nb_token + 1));//check where to free
	if (!argv)
		malloc_error();//check if exit or NULL (may be NULL)
	// current_token = token;
	// i = 0;
	// while (current_token && current_token->type != OPERATOR)
	// {
	// 	argv[i] = ft_strdup(current_token->txt);
	// 	if (!argv[i])
	// 	{
	// 		perror ("Failed ft_strdup token to argv");//check
	// 		free_partial_arr(argv, i);
	// 		return (NULL);//check and make good the return if fails
	// 	}
	// 	current_token = current_token->next;
	// 	i++;
	// }
	// argv[i] = NULL;
	if (copy_token_strings(argv, token) != 0)
	{
		free(argv);
		fprintf(stderr, BOLD CYAN "copy_token_strings FAILED\n" RESET);//TEST
		return (NULL);
	}
	return (argv);
}

// void	pre_execution(t_node *node, char **envp)
void	pre_execution(t_program *program, t_node *node)
{
	// t_token	*curren_token;
	// int		i;
	// int		nb_token;

	if (!node)
		return ;
	if (node->type == OPERATOR)
	{
		pre_execution(node->u_data.op.left, program->envp);
		pre_execution(node->u_data.op.right, program->envp);
	}
	else if (node->type == COMMAND)
	{
		node->u_data.cmd.argv = token_to_argv(node->u_data.cmd.tokens);////init node
		if (!node->u_data.cmd.argv)
		{
			perror(BOLD RED "Failed to create argv" RESET); //check msg
			exit(EXIT_FAILURE);
		}
	node->u_data.cmd.env = program->envp;
	if (node->u_data.cmd.argv[0] && is_builtin(node->u_data.cmd.argv[0]))
		node->u_data.cmd.cmd_type = BUILTIN;
	else
		node->u_data.cmd.cmd_type = EXECUTABLE;//for this test
	node->u_data.cmd.fd_in = STDIN_FILENO;
	node->u_data.cmd.fd_out = STDOUT_FILENO;
	}
}
