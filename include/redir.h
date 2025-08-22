/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redir.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: albetanc <albetanc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/22 11:31:14 by albetanc          #+#    #+#             */
/*   Updated: 2025/08/22 18:34:41 by albetanc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef REDIR_H
# define REDIR_H

# include "minishell.h"

// -----------------------------------------//
//                PROTOTYPES                //
// -----------------------------------------//

t_redir	*create_redir_node(char *target, enum e_redir_type type);
int		open_redir_filename(t_redir *redir);
int		process_redir(t_cmd_data *cmd);
void	restore_std(t_program *program);
int		setup_redir(t_cmd_data *cmd);

#endif
