/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: albetanc <albetanc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/11 11:52:09 by albetanc          #+#    #+#             */
/*   Updated: 2025/08/11 12:27:44 by albetanc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "exec.h"//TMP FOR TESTING

typedef struct s_minishell//new include in minishell.h
{
	//char *line;
	//char	**envp_cpy;//uncomment when is in minishell.h
	//char	**envp;//original not will be free
	//t_node	*root;
	//t_token	*token_list;
	int		last_exit_status;
}	t_program;

//line included to centralized cleanup
void	cleanup_program(t_program *program)//moved to cleanup.c
{//probably includes prompt when is dynamic
	if (program->line)
		free(program->line);
	if (program->token_list)
		free_token(program->token_list);
	if (program->root)
		free_node(program->root);
	if (program->envp_cpy)
		free_array(program->envp_cpy);
}

/*
*	@brief ends the progam and free resources needed: nodes, tokens, etc
*	@returns void because ends the program
*/
void	my_exit(t_program *program)
{
	cleanup_program(program);
	exit(program->last_exit_status);
}

