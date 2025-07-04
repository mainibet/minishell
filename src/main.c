/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: albetanc <albetanc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/23 08:42:03 by albetanc          #+#    #+#             */
/*   Updated: 2025/07/04 12:56:45 by albetanc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

//main v0:
//set-ups the shell's core interactive loop: while(1)
// display prompt and readline command lines with readline
//until the user exits
//includes add_history
int	main(void)
{
	char	*line;
	char	*prompt;

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
		printf(BOLD CYAN "Command received: %s\n" RESET, line);
		free(line);
	}
	return (0);
}
