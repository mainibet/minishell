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

#include "../include/minishell.h"
// #include "../include/lexer.h"
// #include "../include/parser.h"
#include "../include/prexec.h"
#include "../include/exec.h"

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

t_node	*parse_input(char *line, t_token **token_out)
{
	t_token	*token_list;
	t_token	*parser_tokens;
	t_node	*root;
// --- lexer --- //
	token_list = lex(line, ' ');
	if (!token_list)
	{
		fprintf(stderr, BOLD RED "Lexing failed or empty imput\n" RESET);//test
		*token_out = NULL;//test
		return (NULL);
	}
	// fprintf(stderr, MAGENTA "Tokens generated: " RESET);//test
	// print_tokens(token_list);//test
	// fprintf(stderr, MAGENTA"\n" RESET);//test
// --- parser ---//
	parser_tokens = token_list;
	// root = parse(&parser_tokens, 0);//0 is the minimum precedency to begin
	root = parse(parser_tokens);//NEW TEST EXEC
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

static void	handle_command(char *line, char **envp)
{
	t_token	*token_list;
	t_node	*root;

	if (!line)//handles NULL line
		return ;
	if (!*line)//handles empty line
	{
		free(line);
		return ;
	}
	add_history(line);
	root = parse_input(line, &token_list);
	free(line);
	if (!root)//if parsing failed
		return ;
	fprintf(stderr, BOLD MAGENTA "AST built. stating pre-execution \n" RESET);//TEST
	pre_execution(root, envp);
	execution(root);
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
			process_node(&program);//new NODE ROOT NEEDS TO BE FILL BY PARSER
		}
		else if (program.line)
			free(program.line);
	}
	cleanup_program(&program);
	return (program.last_exit_status);
	// return (0);
}
