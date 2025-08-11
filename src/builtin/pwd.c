/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: albetanc <albetanc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/11 09:27:29 by albetanc          #+#    #+#             */
/*   Updated: 2025/08/11 10:01:41 by albetanc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "exec.h"//TMP FOR TESTING

//cwd: current working directory
//Original pwd: Print the full 
//filename of the current working directory.
int	my_pwd(char *cwd_path)
{
	if (cwd_path)
	{
		printf("%s\n", cwd_path);
		return (0);
	}
	else
	{
		fprintf(stderr, BLUE "pwd: current directory not found\n" RESET);
		return (1);
	}
}
