#include "minishell.h"
#include "exec.h"//temporary for testing

/*
*   Parent waits for a specific child process to terminate, using waitpid()
*   To clean up after a child and retrieve its exit status
*/
int	wait_child_status(pid_t pid, int *status)
{
	if (pid <= 0)//not wait if there is not valid pid
		return (0);
	if (waitpid(pid, status, 0) == -1)
	{
		perror (BOLD RED "Error waiting for child" RESET);
		return (-1);
	}
	return (0);
}

/*
*   [ERROR HANDLING]
*   Checks the result of a fork_handle
*   If fork() failed, it might wait for a previously launched child 
*   before returning an error status.
*   To managing the parent's flow after a fork attempt.
*/

int	check_fork(int result, pid_t pid, int *status)//may be not needed with multiple child
{
	if (result != 0)
	{
		if (result == 1)
			return (0);
		else
		{
			if (pid != 0)
				wait_child(pid1, status);
			return (result);
		}
	}
	return (0);
}

/*
*   Forks a new process
*   1. Handles fork errors
*   2. Executes child based on cmd position in a pipeline.
*   3. Exit in case of non sucess in any function to end correctly child
*/
//i_cmd == 0 is first cmd
//i_cmd == nb_cmd -1 last cmd
//return pid to the parent
//if pid fails not fd where inheritated, the parent handles them
int	fork_handle(t_node *node, int i_cmd, int nb_cmd)
{
	pid_t pid;

	pid = fork();
	if (pid == -1)
	{
		perror (BOLD RED "Fork failed" RESET);
		return (-1);//check if smt should be free
	}
	if (pid == 0)
	{
		if (nb_cmd == 1)
			child_process(node);//single cmd
		else if (i_cmd == 0)
			child_first(node);
		else if (i_cmd == nb_cmd -1)
			child_last(node);
		else
			child_middle(node);
		exit(EXIT_FAILURE);
	}
	return (pid);
}


