/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: albetanc <albetanc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/21 16:32:13 by albetanc          #+#    #+#             */
/*   Updated: 2025/07/25 12:24:47 by albetanc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// #include "minishell.h"
#include "../include/minishell.h"
#include "exec.h"//temporary for testing

//---------------------------------------//
//V0:Execute single external cmd         //
//   directly from readline              //
//                                       //
//V1: support external cmd in            //
//    child process (cmd node)           //
//                                       //
//V2: supports pipe_node                 //
//                                       //
//V3: support some built-ins             //
//                                       //
//V4: supports basic parsing             //
//                                       //
//V5: supports all built-ins             //
//                                       //
//V6: support parsing refinement         //
//                                       //
//V7: support heredoc & redirections     //
//                                       //
//V8: support bonus (if needed)          //
//---------------------------------------//

//---------------------------------------//
//                    V0                 //
// GOAL: Executes single external cmd    //
// 1. Read a full line as raw input.     //
// 2. Use getenv to get absolute path    //
// 3. Use execve() to execute in parent  //
//    the input line as-is.              //
// 4. Wait for the child process(waitpid)//
// 5. Exec cmd will exit shell           //
//---------------------------------------//

//---------------------------------------//
//                  V1                   //
// GOAL: Executes single external cmd    //
// 1. Read a full line as raw input.     //
// 2. Use getenv to get absolute path    //
// 3. Use fork() and execve() to execute //
//    the input line as-is.              //
// 4. Wait for the child process(waitpid)//
// 5. Exit command should leave shell    //
//    active                             //
//---------------------------------------//

//---------------------------------------//
//            V2 - CURRENT               //
// GOAL: Executes pipe nodes             //
// 1. Multipipes: 3 CHILD?               //
// 2. COMPLETE THIS                      //
// 3. INCLUDE ECHO EXECUTION? BUILTIN    //
//   fd CONTROL                          //
// 4.traverse CHECK JOSHUA'S             //
// 5.                                    //
//                                       //
//---------------------------------------//
/*
* CLEAN-UP WITH PIPES
* DEFINE TYPES OF CHILD CONSIDERING OTHERS OPERANDS
* BUILTINS FLOW ONLY ECHO
* INCLUDE OPERAN NODE IN EXECUTION FLOW
* CONTROL AND CHECK OPENING AND CLOSE FD
* PIPE AND REDIRECTIO IMPLEMENTATION IN CHILD
* UPDATE NAMES IN ECHO BUILTIN
* CHECK IF PREEXECUTION NEEDS SOMETHING
* CHECK EDGE CASES
* CHECK ALLOWED FUNCTIONS
* UPDATE EXEC SPIDER FOR SINGLE EXTERNAL CMD
* MAKE SPIDER FOR PIPE EXECUTION
*/

/**
*   @brief Execute single external cmd
*
*   1. Find absolut path
*   2. Executes execve in child process
*   3. Handle errors: cmd_pah and execve
*   @return none. executes in success or
*   EXIT_FAILURE if fails
*   -Use called in the child_process
*
*   @note:
*   - handle error of execve only is
*     reached if it fails
*   - cmd_path only filled if there is a 
*     valid path                  
*/

void	exec_external_cmd(t_node *node)
{
	char	*cmd_path;

	cmd_path = find_path (node->u_data.cmd.argv[0]);
	if (!cmd_path)
	{
		perror (BOLD RED "command_path not found" RESET);
		exit(EXIT_FAILURE);
	}
	execve(cmd_path, node->u_data.cmd.argv, node->u_data.cmd.env);
	free(cmd_path);
	exit(EXIT_FAILURE);
}
static int	handle_operator(t_node *node)//NEW
{
	int	left_status;
	int	right_status;

	if (node->u_data.op.type == PIPE)
		return (handle_pipe_node(node));//TODO connect with pipe.c
	else if (node->u_data.op.type == AND)
	{
		left_status = execution(node->u_data.op.left);
		if (left_status == 0)
			return (execution(node->u_data.op.right));
		return (left_status);
	}
	else if (node->u_data.op.type == OR)
	{
		left_status = execution(node->u_data.op.left);
		if (left_status != 0)
			return (execution(node->u_data.op.right));
		return (left_status);
	}
	else if (node->u_data.op.type == SEMICOLON)
	{
		execution(node->u_data.op.left);
		return (execution(node->u_data.op.right));
	}
	fprintf(stderr, CYAN RED "Error: unknow type operand for execution\n" RESET);
	return (1);
}

/**
*	@brief Receives a node from AST and decides 
*   type of execution based on the type node
*
*	1. Base case: (!root) returns
*   2. calls execute_cmd
*
*   @param root The root node of AST to be executed
*
*   @note execution will be performed in recursion
*/
int	execution(t_node *node)//check if change parameter name
{
	if (!node)
		return ;
	if (node->type == COMMAND) 
		execute_cmd(node);
	else if (node->type == OPERATOR)//NEW
		return (handle_operador(node));
	fprintf(stderr, CYAN RED "Error: unknow type node for execution\n" RESET);
	return (1);
}
