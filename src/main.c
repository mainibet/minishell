/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: albetanc <albetanc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/23 08:42:03 by albetanc          #+#    #+#             */
/*   Updated: 2025/07/23 15:16:40 by albetanc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"
#include "../include/lexer.h"
#include "../include/parser.h"
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
	fprintf(stderr, MAGENTA "Tokens generated: " RESET);//test
	print_tokens(token_list);//test
	fprintf(stderr, MAGENTA"\n" RESET);//test
// --- parser ---//
	parser_tokens = token_list;
	root = parse(&parser_tokens, 0);//0 is the minimum precedency to begin
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

int	main(int argc, char **argv, char **envp)
{
	char	*line;
	char	*prompt;

	(void) argc;//check if needed
	(void) argv;//check if needed

	prompt = BOLD GREEN "🐶🥕 Milanshell >" RESET;
	while (1)
	{
		line = readline(prompt);
		if (!line && isatty(STDIN_FILENO)) //if issaty returns 0 is in an fd
		{
			printf(BLUE "exit\n" RESET);
			break ;
		}
		printf(BOLD MAGENTA "Command received: %s\n" RESET, line);//test
		handle_command(line, envp);//new
	}
	return (0);
}
