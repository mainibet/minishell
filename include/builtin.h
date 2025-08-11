/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: albetanc <albetanc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/31 17:54:38 by albetanc          #+#    #+#             */
/*   Updated: 2025/08/11 14:15:13 by albetanc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef BUILTIN_H
#define BUILTIN_H

# include "minishell.h"


typedef enum e_builtin_type
{
	BUILTIN_NONE = 0,//default if is not
	BUILTIN_ECHO,
	BUILTIN_CD,
	BUILTIN_PWD,
	BUILTIN_EXPORT,
	BUILTIN_UNSET,
	BUILTIN_ENV,
	BUILTIN_EXIT,
	// MAX_BUILTIN_TYPE//usefull to loop or validations
}	t_builtin_type;

// -----------------------------------------//
//                PROTOTYPES                //
// -----------------------------------------//

int		is_builtin(const char *cmd_name);
int		execute_builtin(t_node *node);

// --- libft --//
int ft_strcmp(const char *s1, const char *s2);//include to libft

// --- SPECIFICS --- //

int	execute_builtin(t_context *program, t_node *node);
int	my_echo(t_node *node);
int	my_pwd(char *cwd_path);
// int	my_env(char **envp);
int	my_env(t_context *program, t_node *node);
int	my_export(t_context *program, t_node *node);
// int	my_export(char **envp);

#endif