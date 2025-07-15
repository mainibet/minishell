/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: albetanc <albetanc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/15 08:36:45 by albetanc          #+#    #+#             */
/*   Updated: 2025/07/15 15:46:28 by albetanc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LEXER_H
# define LEXER_H

# include "include/minishell.h"

// -----------------------------------------//
//                  STRUCTS                 //
// -----------------------------------------//

typedef struct s_token
{
	char			*txt;
	t_token			*next;
	enum e_toktype	type;
	char			delim;
}	t_token;


// -----------------------------------------//
//                PROTOTYPES                //
// -----------------------------------------//

t_token			*lex(char *s, char delim);
void			free_token(t_token *token);
enum e_toktype	token_type(t_token *token);
t_token			*extract_token(char *s, size_t size);

#endif