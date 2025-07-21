/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prexec.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: albetanc <albetanc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/16 08:41:11 by albetanc          #+#    #+#             */
/*   Updated: 2025/07/21 11:11:36 by albetanc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

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

char	**token_to_argv(t_token *token)
{
	char	**argv;
	t_token	*current_token;
	int		nb_token;
	int		i;

	nb_token = count_tokens(token);
	argv = malloc(sizeof(char *) * (nb_token + 1));//check where to free
	if (!argv)
		malloc_error();//check if exit or NULL (may be NULL)
	current_token = token;
	i = 0;
	while (current_token)
	{
		argv[i] = ft_strdup(current_token->txt);
		if (!argv[i])
		{
			perror ("Failed ft_strdup token to argv");//check
			free_partial_arr(argv, i);
			return (NULL);//check and make good the return if fails
		}
		current_token = current_token->next;
		i++;
	}
	argv[i] = NULL;
	return (argv);
}

void	pre_execution(t_node *node, char **envp)
{
	t_token	*curren_token;
	int		i;
	int		nb_token;

	if (!node)
		return ;
	if (node->type != COMMAND)//tmp
		return ;
	// (node->type == COMMAND)//late
	node->u_data.cmd.argv = token_to_argv(node->u_data.cmd.tokens);
	// else if (node->type == OPERATOR)//later
	// {
	// 	re_execution(node->u_data.op.left, envp);
	// 	re_execution(node-?u_data.op.rith, envp);
	// }
	if (!node->u_data.cmd.argv)
		exit(EXIT_FAILURE);//check to handle error correctly
	node->u_data.cmd.env = envp;
	node->u_data.cmd.cmd_type = EXECUTABLE;//for this test
	node->u_data.cmd.fd_in = STDIN_FILENO;
	node->u_data.cmd.fd_out = STDOUT_FILENO;
}
