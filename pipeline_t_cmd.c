#include "minishell.h"

void close_pipe(int pipefd[2])
{
	close(pipefd[0]);
	close(pipefd[1]);
}

void try_dup2(int pipe_end, int stdio)
{
	if(dup2(pipe_end, stdio) == -1)
	{
		perror("dup2 failed");
		exit(1);
	}
}

void try_pipe(int pipefd[2])
{
	if (pipe(pipefd) == -1)
	{
		perror("pipe failed");
		exit(1);
	}
}

pid_t try_fork()
{
	pid_t pid = fork();
	if (pid < 0)
	{
		perror("fork failed");
		exit(1);
	}
	return (pid);
}

void try_wait(pid_t pid, char *path)
{
	int wstatus;
	waitpid(pid, &wstatus, 0);
	if(!WIFEXITED(wstatus) || WEXITSTATUS(wstatus) != 0)
	{
		perror(path);
		exit(1);
	}
}

/* cmd is a null terminated list of t_cmd in a pipeline, from *last to first*
 * pipe_after is an empty pair of descriptors that we must pipe()
 */
void pipeline(t_cmd *cmd, int *pipe_after)
{
	int pipe_before[2]; // empty space for the pipe before
	pid_t pid;
	//char *argv[] = {paths[0], NULL};
	//char *env[] = {NULL};
	
	if (paths[1]) // this is not the first command in the pipeline
		pipeline(&paths[1], pipe_before);
	if (pipe_after) // this is not the last command in the pipeline
		try_pipe(pipe_after);
	pid = fork();
	if (!pid)
	{
		if(paths[1])
		{
			try_dup2(pipe_before[0], STDIN_FILENO);
			close_pipe(pipe_before);
		}
		if(pipe_after)
		{
			try_dup2(pipe_after[1], STDOUT_FILENO);
			close_pipe(pipe_after);
		}
		execve(cmd->path, cmd->argv, cmd->env);
		perror(cmd->path);
	}
	try_wait(pid, cmd->path);
	if(pipe_after)
		close(pipe_after[1]);
	if(cmd[1])
		close(pipe_before[0]);
	return ;
}
