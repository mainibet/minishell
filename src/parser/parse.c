/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: albetanc <albetanc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/15 13:35:57 by albetanc          #+#    #+#             */
/*   Updated: 2025/07/15 13:56:10 by albetanc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "parser.h"

// struct s_node;//moved to minishell.h

// typedef struct s_operator//moved to parser.h
// {
// 	enum e_toktype	type;
// 	struct s_node	*left;
// 	struct s_node	*right;
// }	t_operator;

// enum e_nodetype
// {
// 	OPERATOR,
// 	TERMINAL,
// 	MAX_NODETYPE
// };

// union u_node//included in parser.h
// {
// 	t_operator	op;
// 	t_token		*tokens;
// };

// typedef struct s_node //included in parser.h
// {
// 	enum e_nodetype	type;
// 	union u_node	content;
// }	t_node;

static void	free_node(t_node *node)
{
	if (node->type == OPERATOR)
	{
		free_node(node->content.op.left);
		free_node(node->content.op.right);
	}
	free(node);
}

static t_token	*next_operator(t_token *token)
{
	while (token->next && token->type < PIPE)
		token = token->next;
	return (token);
}

static int	precedence(t_token *token)
{
	if (token->type == SEMICOLON)
		return (1);
	if (token->type == AND || token->type == OR)
		return (2);
	if (token->type == PIPE)
		return (3);
	return (0);
}

static t_node	*parse_terminal(t_token *token)
{
	t_node	*node;

	node = malloc(sizeof(t_node));
	if (!node)
		return (NULL);
	node->type = TERMINAL;
	node->content.tokens = token;
	return (node);
}

static t_node	*parse_operator(t_token *op, t_node *left, t_node *right)
{
	t_node	*node;

	node = malloc(sizeof(t_node));
	if (!node)
	{
		free_node(left);
		free_node(right);
	}
	node->type = OPERATOR;
	node->content.op.type = token_type(op);
	node->content.op.left = left;
	node->content.op.right = right;
	return (node);
}

/* take the address of a token list and create a parse tree.
 * - all pipes are treated as right-associative.  this means we only need to keep
 *   3 open fds at any given time.
 * - all other operators are left-associative
 */
t_node	*parse(t_token **token, int min_precedence)
{
	t_token	*op;
	t_node	*left;
	t_node	*right;

	left = parse_command(*token);
	if (!left)
		return (NULL);
	*token = next_operator(*token);

	while (*token && (*token)->type >= PIPE && precedence(*token) >= min_precedence)
	{
		op = *token;
		*token = (*token)->next;
		if ((*token)->type == PIPE)
			right = parse(token, precedence(op)); // right-associative for pipes
		else
			right = parse(token, precedence(op) + 1);
		if (!right)
		{
			free_node(left);
			return (NULL);
		}
		left = parse_operator(op, left, right);
		if (!left)
			return (NULL);
	}
	return (left);
}

/* execute the command or builtin specified by tokens
 * - the child (if any) should close each element of fd that is greater than 2
 * 		(not stdout, stdin, or stderr)
 * - the parent should only close fd[1] and fd[2] after waitpid().
//  */
// int	exec(t_token *tokens, int fd[3], char **env)
// {
// 	char	*path;
// 	char	**argv;

// 	if (is_builtin(tokens))
// 		return (exec_builtin(tokens));
// 	parse_command(tokens, path, argv)
// 	pid = fork();
// 	if (!pid)
// 	{
// 		if (fd[0] > 2)
// 		{
// 			dup2(STDIN_FD, fd[0]);
// 			close(fd[0]);
// 		}
// 		if (fd[1] > 2)
// 		{
// 			dup2(STDOUT_FD, fd[1]);
// 			close(fd[1]);
// 		}
// 		if (fd[2] > 2)
// 			close(fd[2]);
// 		execve(path, argv, env);
// 		perror(path);
// 	}
// 	ret = waitpid(pid);
// 	if (fd[0] > 2)
// 		close(fd[0]);
// 	if (fd[1] > 2)
// 		close(fd[1]);
// 	return (get_return_code(ret));
// }

/* @brief recursively traverse a node in an AST, left to right
 * @param node the node to traverse
 * @param fd array of three fds
 *  - fd[0] is the fd to read from
 *  - fd[1] is the fd to write to
 *  - fd[2] if > 2, is the other half of fd[1]'s pipe: it should be
 *  	closed in the child process but not the parent
 *  if node is another operator:
 *  	if node is a pipe:
 *  		- create innerfd as a pipe
 *  	otherwise, innerfd = {0, 1};
 *  	for left:
 *  		fd[0] = fd[0];
 *  		fd[1] = innerfd[1];
 *  		fd[3] = innerfd[0];
 *  	for right:
 *  		fd[0] = innerfd[0];
 *  		fd[1] = fd[1];
 *  		fd[3] = 0;
 *	if the node is terminal, pass on the tokens and fds to exec
 */
// int	traverse(t_node *node, int fd[3])//function foe execution
// {
// 	int	innerfd[2];
// 	int outerfd[2];
// 	int	ret;

// 	if (node->type == TERMINAL)
// 		return (exec(node->content.tokens))
// 	outerfd[0] = fd[0];
// 	outerfd[1] = fd[1];
// 	if (node->content.op.type == PIPE)
// 		try_pipe(innerfd);
// 	else
// 	{
// 		innerfd[0] = 0;
// 		innerfd[1] = 1;
// 	}
// 	fd[1] = innerfd[1];
// 	fd[2] = innerfd[0]; // to be closed in the child process
// 	ret = traverse(node->content.op.left, fd);
// 	fd[1] = outerfd[1];
// 	if (node->op.type == AND && !ret)
// 		return (0);
// 	if (node->op.type == OR && ret)
// 		return (ret);
// 	fd[0] = innerfd[0];
// 	fd[2] = 0; // nothing to close on the right
// 	ret = traverse(node->content.op.right, fd);
// 	fd[0] = outerfd[0];
// 	return (ret);
// }
