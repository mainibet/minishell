/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: albetanc <albetanc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/23 08:42:03 by albetanc          #+#    #+#             */
/*   Updated: 2025/07/16 09:50:02 by albetanc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

//-------------------------------//
//          V0: main             //
//                               //
// set-ups the shell's core      //
// interactive loop: while(1)    //
// display prompt and readline   //
// command lines with readline   //
// until the user exits          //
// includes add_history          //
//-------------------------------//

int	main(int argc, char **argv, char **envp)
{
	char	*line;
	char	*prompt;
	t_token	*token_list;
	t_node	*root;

	(void) argc;//check if needed
	(void) argv;//check if needed

	prompt = BOLD GREEN "🐶🥕 Milanshell >" RESET;
	while (1)
	{
		line = readline(prompt);
		if (line == NULL)
		{
			printf(BLUE "exit\n" RESET);
			break ;
		}
		if (*line)
			add_history(line);
		printf(BOLD MAGENTA "Command received: %s\n" RESET, line);//test
		// --- lexing --- //
        token_list = lex(line, ' ');
		if (token_list)
		{
			free(line);
			continue ;
		}
	}
	return (0);
}
