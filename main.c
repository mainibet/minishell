/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: albetanc <albetanc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/23 08:42:03 by albetanc          #+#    #+#             */
/*   Updated: 2025/06/23 08:44:50 by albetanc         ###   ########.fr       */
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

	prompt = "🐶🥕 Milanshell >";
	while (1)
	{
		line = readline(prompt);
		if (line == NULL)
		{
			printf("exit\n");
			break ;
		}
		if (*line)
			add_history(line);
		printf("Command received: %s\n", line);
		free(line);
	}
	return (0);
}
