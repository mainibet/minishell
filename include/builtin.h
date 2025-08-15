/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rococo <rococo@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/31 17:54:38 by albetanc          #+#    #+#             */
/*   Updated: 2025/08/15 13:20:03 by rococo           ###   ########.fr       */
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
}	t_builtin_type;

// -----------------------------------------//
//                PROTOTYPES                //
// -----------------------------------------//

int		is_builtin(const char *cmd_name);
int		execute_builtin(t_program *program, t_node *node);

// --- SPECIFICS --- //
int		my_echo(t_program *program, t_node *node);
int		my_pwd(t_program *program, t_node *node);
int		my_env(t_program *program, t_node *node);
int		my_export(t_program *program, t_node *node);
void	my_exit(t_program *program, t_node *node);;

#endif