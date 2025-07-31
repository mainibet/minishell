/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: albetanc <albetanc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/31 17:54:38 by albetanc          #+#    #+#             */
/*   Updated: 2025/07/31 18:33:30 by albetanc         ###   ########.fr       */
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
	// MAX_BUILTIN_TYPE//check
}	t_builtin_type;

// -----------------------------------------//
//                PROTOTYPES                //
// -----------------------------------------//

int		is_builtin(const char *cmd_name);
int		execute_builtin(t_node *node);

// --- SPECIFICS --- //

void	my_echo(t_node *node);

#endif