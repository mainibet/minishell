/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: albetanc <albetanc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/23 08:42:03 by albetanc          #+#    #+#             */
/*   Updated: 2025/08/11 13:59:45 by albetanc         ###   ########.fr       */
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

//-------------------------------//
//         V1: CURRENT           //
//                               //
// Calls lexer, parser, prexec   //
// and execution.......          //
//-------------------------------//

t_node	*token_parser_input(char *line, t_token **token_out)
{
	t_token	*token_list;
	t_token	*parser_tokens;
	t_node	*root;

	token_list = lex(line, ' ');
	if (!token_list)
	{
		fprintf(stderr, BOLD RED "Lexing failed or empty imput\n" RESET);//test
		*token_out = NULL;
		return (NULL);
	}
	parser_tokens = token_list;
	root = parse(parser_tokens);
	if (!root)
	{
		fprintf(stderr, "Parsing failed\n");
		free_token(token_list);
		*token_out = NULL;
		return (NULL);
	}
	*token_out = token_list;
	return (root);
}

static void	process_cmdline(t_program *program, char *line)
{
	t_token	*token_list;
	t_node	*root;

	if (!line)
		return ;
	if (!*line)
	{
		free(line);
		return ;
	}
	add_history(line);
	root = token_parser_input(line, &token_list);
	free(line);
	if (!root)//if parsing failed
		return ;
	fprintf(stderr, BOLD MAGENTA "AST built. stating pre-execution \n" RESET);//TEST
	pre_execution(program, root);
	execution(program, root, false);
	free_token(token_list);
	free_node(root);
	fprintf(stderr, BOLD MAGENTA "Command processed and cleaned up\n" RESET); //TEST
}

void	init_program(t_program *program, char **envp)
{
	program->line = NULL;
	program->root = NULL;
	program->token_list = NULL;
	program->envp = envp;
	program->envp_cpy = NULL;
	program->last_exit_status = 0;
}

int	main(int argc, char **argv, char **envp)
{
	char		*prompt;
	t_program	program;//new for builtin exit

	(void) argc;//check if needed
	(void) argv;//check if needed

	init_program(&program, envp);
	prompt = BOLD GREEN "🐶🥕 Milanshell >" RESET;
	while (1)
	{
		program.line = readline(prompt);//new program.
		if (!program.line && isatty(STDIN_FILENO)) //if issaty returns 0 is in an fd
		{
			printf(BLUE "exit\n" RESET);
			break ;
		}
		if (program.line && *program.line)
		{
			add_history(program.line);
			printf(BOLD MAGENTA "Command received: %s\n" RESET, program.line);//test
			process_node(&program);//this will be the root
		}
		else if (program.line)
			free(program.line);
	}
	cleanup_program(&program);
	return (program.last_exit_status);
}
