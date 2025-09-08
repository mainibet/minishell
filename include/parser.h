/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: albetanc <albetanc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/15 13:34:47 by albetanc          #+#    #+#             */
/*   Updated: 2025/09/08 15:30:41 by albetanc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSER_H
# define PARSER_H

# include "minishell.h"

// -----------------------------------------//
//                PROTOTYPES                //
// -----------------------------------------//

t_node	*parse(t_token *token_list);
void	init_cmd_node(t_token *token, t_node *node);
int		process_cmd_tokens(t_token *token, t_cmd_data *cmd_data);

// --- Tokens --- //
int		process_tokens_loop(t_token *token, t_cmd_data *cmd_data,
			t_token **cmd_tokens);

// --- Arguments --- //
int		split_cmd_arg(t_token **current, t_cmd_data *cmd_data);

#endif