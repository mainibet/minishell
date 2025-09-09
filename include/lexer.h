/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: albetanc <albetanc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/15 08:36:45 by albetanc          #+#    #+#             */
/*   Updated: 2025/09/09 17:27:43 by albetanc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LEXER_H
# define LEXER_H

# include "minishell.h"

// -----------------------------------------//
//                PROTOTYPES                //
// -----------------------------------------//

t_token			*lex(char *s, int is_delim);
enum e_toktype	token_type(char *s);
t_token			*join_tokens(t_token *a, t_token *b);

// --- Quotes --- //
t_token			*lex_quoted(char *s, char quote);
t_token			*lex_unquoted(char *s);
t_token			*extract_token(char *s, size_t size);

// --- Helpers --- //
int				ft_isspace(char c);
char			*consume_whitespace(char *head_token);
int				is_operator_char(char c);

#endif
