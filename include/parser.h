/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: albetanc <albetanc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/15 13:34:47 by albetanc          #+#    #+#             */
/*   Updated: 2025/07/15 13:54:13 by albetanc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSER_H
# define PARSER_H

#include "minishell.h"

// ---FORWARD DECLARATIONS--- //


// -----------------------------------------//
//                  STRUCTS                 //
// -----------------------------------------//

typedef struct s_operator
{
	enum e_toktype	type;
	struct s_node	*left;
	struct s_node	*right;
}	t_operator;

union u_node
{
	t_operator	op;
	t_token		*tokens;
};

typedef struct s_node 
{
	enum e_nodetype	type;
	union u_node	content;
}	t_node;

// -----------------------------------------//
//                PROTOTYPES                //
// -----------------------------------------//
t_node	*parse(t_token **token, int min_precedence);

#endif