/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   path.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: albetanc <albetanc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/30 17:13:58 by albetanc          #+#    #+#             */
/*   Updated: 2025/06/30 17:14:31 by albetanc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char	**get_path(void)
{
	char	*path;
	char	**dir;

	path = getenv("PATH"); // new function allowed in minishell
	if (!path)
	{
		perror("PATH not found");
		return (NULL);
	}
	dir = ft_split(path, ':');
	return (dir);
}

// Joins a directory and a filename to create a full file path.
static char	*create_path(char *dir, char *argv)
{
	char	*each_path;
	char	*file_path;

	each_path = ft_strjoin(dir, "/");
	if (!each_path)
	{
		free (dir);
		return (NULL);
	}
	file_path = ft_strjoin(each_path, argv);
	free (each_path);
	if (!file_path)
	{
		free(dir);
		return (NULL);
	}
	return (file_path);
}

// Checks if the file at file_path exists; frees memory and returns the path if found.
static char	*check_path(char **dir, char *file_path, int i)
{
	if (access (file_path, F_OK) == 0)
	{
		while (dir[i])
		{
			free(dir[i]);
			i++;
		}
		free (dir);
		return (file_path);
	}
	return (NULL);
}

// Frees all directory strings from index i onwards and returns NULL.
static char	*free_prev_dir(char **dir, int i)
{
	while (dir[i])
	{
		free(dir[i]);
		i++;
	}
	free(dir);
	return (NULL);
}

// Finds the full path of a command by searching directories in PATH.
char	*find_path(char *argv, char **envp)
{
	char	*file_path;
	char	**dir;
	char	*final_path;
	int		i;

	dir = get_path(envp);
	if (!dir)
		return (NULL);
	i = 0;
	while (dir[i])
	{
		file_path = create_path(dir[i], argv);
		if (!file_path)
			free_prev_dir(dir, i);
		final_path = check_path(dir, file_path, i);
		if (final_path)
			return (final_path);
		free(file_path);
		free (dir[i]);
		i++;
	}
	free(dir);
	return (NULL);
}