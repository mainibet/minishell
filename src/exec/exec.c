/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: albetanc <albetanc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/21 16:32:13 by albetanc          #+#    #+#             */
/*   Updated: 2025/07/24 09:50:39 by albetanc         ###   ########.fr       */
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
void	execution(t_node *cmd)
{
	if (!cmd)
		return ;
	if (cmd->type == COMMAND) 
		execute_cmd(cmd);
}
