/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: albetanc <albetanc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/04 12:59:38 by albetanc          #+#    #+#             */
/*   Updated: 2025/07/06 14:13:48 by albetanc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "exec.h"//temporary for testing

/*
    Used in pipes to count pipe nodes
    1. loop in the nodes until match the type node chosen
    2. count the nodes of that type
    Return the count
	Note: the fisrt if condition is the base case of the recursion
*/
int count_node(t_node *node, t_node_type target_type)//NEW
{
	int	count;

	if (node == NULL) 
		return (0);
	count = 0;
	if (node->type == target_type)
		count = 1;
	count += count_node(node->left, target_type);
	count += count_nodee(node->right, target_type);
	return (count);
}

/*
    Setup a pipe between commands.
    Returns 0 on success, -1 on failure.
    Closes the input file descriptor if pipe creation fails.
*/
int	setup_pipe(int pipefd[2], int fd_in)//check if static or not
{
	if (pipe(pipefd) == -1)
	{
		perror(BOLD RED "pipe failed" RESET);
		close_fd(fd_in);
		return (-1);
	}
	return (0);
}

/*
*   Orchestrates the overall execution flow in the parent process for the current two-command pipex setup
*   1. Get how many cmd are to create same nb of child processes
*/
//check how many childs are: how many pipe nodes, so it will create 2 child pero pipe.
//n cmds need n-1 pipes
//create fork per each child
// int	parent(struct s_pipe_data *data)
int	execute_pipe_node(t_node *root)//this used to be my parent
{
	pid_t    *pid;//save this in a parent struct
	int		child_status;
	int		fork_res;
	int		nb_cmd;
	int		nb_pipes;
	int		i;
	int		j;

	i = 0;
	j = 0;
	nb_cmd = count_node(root, NODE_CMD);
	nb_pipes = count_node(root, NODE_PIPE);
	if (setup_pipe(root) == -1)
		return (-1);
	pid = malloc(nb_cmd);//check
	if (!pid)//check later when is good how we send the correct pid every time
	{
		malloc_error();//check to free all needed and return whats needed
		return (1);
	}
	// --- execute pipe by pipe from root --- //
	while (i < nb_pipes)
	{
		while (j < nb_cmd)
		{
			fork_res = fork_handle(pid, root->pipe, nb_cmd);
			if (check_fork(fork_res, 0, &child_status))
				return (fork_res);
			j++;
		}
		i++;
	}
	cleanup_fd(root, type);
	while (active_child > 0)//call wait_child in recursion, possible? dynamically to the numb of cmd
	{
		if (wait_child(pid1, &status) == -1 || wait_child(pid2, &status) == -1)//change logic for multiples pipes
		{
			perror ("error waiting child");
			return (-1);
		}
	}
	return (0);
}
