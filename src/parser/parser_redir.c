/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_redir.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: albetanc <albetanc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/08 14:31:58 by albetanc          #+#    #+#             */
/*   Updated: 2025/09/12 16:55:28 by albetanc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// char	**copy_token_strings(t_token *cmd_tokens, char **argv)
// {
// 	t_token	*current;
// 	int		i;

// 	i = 0;
// 	current = cmd_tokens;
// 	while (current)
// 	{
// 		argv[i] = ft_strdup(current->txt);
// 		if (!argv[i])
// 		{
// 			free_array(argv);
// 			return (NULL);
// 		}
// 		i++;
// 		current = current->next;
// 	}
// 	argv[i] = NULL;
// 	return (argv);
// }

// static int	ft_lstsize(t_token *lst)
// {
// 	int	count;

// 	count = 0;
// 	while (lst)
// 	{
// 		count++;
// 		lst = lst->next;
// 	}
// 	return (count);
// }

// char	**build_argv_from_tokens(t_program *program, t_token *cmd_tokens)
// {
// 	char	**argv;

// 	argv = malloc(sizeof(char *) * (ft_lstsize(cmd_tokens) + 1));
// 	if (!argv)
// 	{
// 		free_token(cmd_tokens);
// 		program->last_exit_status = 1;
// 		return (NULL);
// 	}
// 	argv = copy_token_strings(cmd_tokens, argv);
// 	if (!argv)
// 	{
// 		free_token(cmd_tokens);
// 		program->last_exit_status = 1;
// 		return (NULL);
// 	}
// 	return (argv);
// }
