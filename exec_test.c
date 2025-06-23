//This is just a test file for the execution part

#include "minishell.h"

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
			break;
		}
		if (*line)
			add_history(line);    
		printf("Comand received: %s\n", line);
		free(line);
	}
	return (0);
}