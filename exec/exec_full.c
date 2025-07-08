
#include "minishell.h" 
 #include "exec.h"//temporary for testing

//---------------------------------------//
//V0:Execute single commands directly,   //
//   from readline                       //
//                                       //
//V1:supports basic parsing              //
//                                       //
//V2:support parsing refinement          //
//                                       //
//V3:execution with pipes & child pcs    //
//                                       //
//V4: support all built-ins              //
//                                       //
//V5: support heredoc & redirections     //
//---------------------------------------//

// Read a full line as raw input.
// Use fork() and execvp() to execute the input line as-is.
// Wait for the child process (waitpid).
// Exit command ends the shell.
// Should check for builtins commands if not, look for external commands ($PATH)

// need to duplicate the arguments (char **argv) before passing them to the child process 
//duplicating the arguments gives the child process its own isolated and safe copy of the command-line arguments. 
//  This prevents unintended side effects and memory issues between the parent and child processes, 
//  particularly crucial when execve() is about to replace the child's entire memory space.

//VALORACION TMP PARA EMPEZAR
//  For each external command in that list, you will:
// Fork a new child process.
// Set up redirections (using adapted setup_redir and redir_input/output).
// Execute the command using execve (similar to your execution function).
// In the parent, wait for that child (using wait_child). This generic, looped approach is what allows Minishell to handle N commands.

//Initially I received a string with the cmd, flags and args
//With the tree I tree node and envp
// void	execution(char	**nargv, char **const envp)
//V0 just executes single cmd
//WHat tokenzation did: char *argv[] = {"ls", "-l", "/home", NULL};
//argv[0] is command name

/*
*   Executes single external cmd
*   1. 0 elements in argv is cmd
*   2. Find absolut cmd path
*   3. Fork
*   4. Waitpit
*   5. Executes cmd
*/
/*
cmd_name
maybe in parser: node->u_data.cmd.cmd_type = get_cmd_type(node->u_data.cmd.argv[0]);
    BUILIN IDEAS

    - child process will check if is builtin to execute
    - if is builtin will find which is it
    - Will execute it accordingly
*/

//-------------------------------//
//         EXECUTION   		     //
//-------------------------------//



/*
*   Execute single external cmd
*   1. find absolut path
*   2. 
*/
void	exec_external_cmd(t_node *node)
{
	char	*cmd_path;

	cmd_path = find_path (node->u_data.cmd.argv[0]);
	if (!cmd_path)
	{
		perror (BOLD RED "command_path not found" RESET);
		//TODO: needed free if applies
		exit(EXIT_FAILURE);
	}
	node->u_data.cmd.argv[0] = cmd_path;//only filled if there is a valid path
	execve(cmd_path, node->u_data.cmd.argv,node->u_data.cmd.env);
	free(cmd_path);//this in case of execution fails.
	//TODO check if free smt else
	exit(EXIT_FAILURE);
}
//TODO: include needed free in the general clean-up or may be another clean_up memory


//---------------------------------------//
//                                       //
//          EXECUTE_NODE                 //
//                                       //
//V0:support external cmd                //
//                                       //
//V1: supports cmd_node                  //
//                                       //
//V1:supports pipe_node                  //
//                                       //
//V2:support bonus                       //
//---------------------------------------//
/*
*	Receives a node from AST and decides type of execution
*	Based on the type node
*	1. Handle cmd node
*	2. Handle pipe node
*/
void	execution(t_node *root)
{
	if (!root)
		return; // Base case for recursion
	if (root->type == NODE_CMD) 
		execute_cmd(root);
	// else if (root->type == NODE_PIPE)
	// 	execute_pipe_node(root);
}