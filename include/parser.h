/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: albetanc <albetanc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/15 13:34:47 by albetanc          #+#    #+#             */
/*   Updated: 2025/09/08 18:46:52 by albetanc         ###   ########.fr       */
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

// --- Parse cmd helpers ---//
int		validate_word_token(t_token *token);
int		validate_redir_target(t_token *current);
int		handle_miss_cmd(t_cmd_data *cmd_data, t_token *cmd_tokens,
			bool has_cmd);

// --- Node creation --- //
t_node	*parse_command(t_token *token);
t_node	*parse_operator(t_token *op, t_node *left, t_node *right);

// --- Tokens --- //
int		process_tokens_loop(t_token *token, t_cmd_data *cmd_data,
			t_token **cmd_tokens);

// --- Arguments --- //
int		split_cmd_arg(t_token **current, t_cmd_data *cmd_data);

#endif
