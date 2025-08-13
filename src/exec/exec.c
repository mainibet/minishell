/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: albetanc <albetanc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/21 16:32:13 by albetanc          #+#    #+#             */
/*   Updated: 2025/08/11 14:04:30 by albetanc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// #include "minishell.h"
#include "../include/minishell.h"
#include "exec.h"//temporary for testing
# include "builtin.h"

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
*   -Use called in the execute_in_child
*
*   @note:
*   - handle error of execve only is
*     reached if it fails
*   - cmd_path only filled if there is a 
*     valid path                  
*/
//this is in child from pipes so no fork need
int	execute_simple_cmd(t_program *program, t_node *cmd_node)
{
	if (is_builtin(cmd_node->u_data.cmd.argv[0]))
	{
		// my_echo(cmd_node);//in the meantime only with this
		return (execute_builtin(program, cmd_node));
		// return (0);
	}
	else
	{
		exec_external_cmd(cmd_node);
		exit (EXIT_FAILURE);
	}
	return (1);
}

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

static int	handle_operator(t_program *program, t_node *node)//NEW
{
	int	left_status;
	int	right_status;

	if (node->u_data.op.type == PIPE)
		return (execute_pipeline(program, node));//TODO connect with pipe.c and program
	else if (node->u_data.op.type == AND)
	{
		left_status = execution(program, node->u_data.op.left);
		if (left_status == 0)
			return (execution(program, node->u_data.op.right));
		return (left_status);
	}
	else if (node->u_data.op.type == OR)
	{
		left_status = execution(program, node->u_data.op.left);
		if (left_status != 0)
			return (execution(program, node->u_data.op.right));
		return (left_status);
	}
	else if (node->u_data.op.type == SEMICOLON)
	{
		execution(program, node->u_data.op.left);
		return (execution(program, node->u_data.op.right));
	}
	fprintf(stderr, BOLD RED "Error: unknow type operand for execution\n" RESET);
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

int	execution(t_program *program, t_node *node)//check if change parameter name
{
	int	status;

	fprintf(stderr, MAGENTA BOLD "About to dispatch command\n" RESET);//test
	if (!node)
	{
		program->last_exit_status = 0;//new my_exit
		return (0);
	}
	if (node->type == COMMAND) 
	{
		status = handle_cmd_exec(program, node, false);//NEW
		fprintf(stderr, MAGENTA BOLD "Will be a cmd\n" RESET);//test
		// return (execute_cmd(node));//only external cmmd
		// return (handle_cmd_exec(node, false));//builtin and external cmd
	}
	else if (node->type == OPERATOR)
	{
		status = handle_operator(program, node);//new
		fprintf(stderr, MAGENTA BOLD "Will be a oprator\n" RESET);//test
		// return (handle_operator(node));
	}
	else
	{
		fprintf(stderr, BOLD RED "Error: unknow type node for execution\n" RESET);
		status = 1;
	}
	program->last_exit_status = status;
	return (status);
}

//to be called from main after parsing
// will prepare the cmds for execution and executions
int	process_node(t_program *program)//new
{
	int	status;

	status = pre_execution(program);
	if (status != 0)
	{
		fprintf(stderr, BOLD CYAN "There was an error in pre-execution" RESET);
		free(program->line);//chek if also free token_list
		free_node(program->root);
		return (status);
	}
	status = execution(program, program->root);
//todo cleanup cmd
	return (status);
}
