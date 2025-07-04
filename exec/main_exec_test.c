#include "minishell.h"
#include "exec.h"

//  --- FILE ONLY FOR TESTING EXECUTION --- //

/*
   v0: call execution for external cmd
   v1: call parent process to execut single cmd
*/

// -----------------------------//
//       MAIN FOR TESTING 		//
// -----------------------------//


/*
        --- VERSION 0 ---
    1. Reads a line from stdin
    2. Set the prompt
    3. It uses readline for input and adds history support.
    4. tokenizes args from cmd line
    5. Creates a command node,
    6. Fills the command node with arguments
    7. Executes the command.
    NOTE: command not execute in milanshell.
*/

/*
  next steps:
   - Implement 1 fork for execute 1 command. GOAL: keep alife minishell
   
*/
int	main(int argc, char **argv, char **envp)
{
	char	*line;
	char	*prompt;
	t_node	*root;//TEST
	char    **parsed_tokens;//TEST

	(void)argc; // TEST
	(void)argv; // TEST
	prompt = GREEN BOLD "🐶🥕 Milanshell >" RESET;
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
		parsed_tokens = parse_line_to_tokens(line, " "); //TEST
		if (parsed_tokens == NULL) //TEST
		{
			free(line); 
			continue; //CHECK IF CAN BE USED and how it works
		}
		root = create_node(NODE_CMD); 
		if (!root)//TEST
			exit_error_program(BOLD RED "Failed to create command node" RESET, line, NULL, NULL);//TEST
		if (fill_command_node(root, parsed_tokens) == -1)//TEST 
			exit_error_program(BOLD RED "Failed to fill command node argv" RESET, line, NULL, root);//TEST
		execution(root, envp);//V0
		free(parsed_tokens);
		free(line);
	}
	// TODO clean-up all nodes and free memory
	return (0);
}
