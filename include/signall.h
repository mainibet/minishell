/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signall.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: albetanc <albetanc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/06 11:39:00 by albetanc          #+#    #+#             */
/*   Updated: 2025/09/06 11:41:05 by albetanc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SIGNALL_H
# define SIGNALL_H

# include "minishell.h"
// #include <readline/readline.h>//already in minishell.h

extern	sig_atomic_t g_signal_value;

void	sigint_prompt(int signum);

void	sigint_heredoc(int signum);

void	set_signal_handler(int signum, void(*handler)(int));

// void	set_signal_prompt(void);
void	set_signal_prompt(int in_heredoc);

void	set_signal_child_process(void);

void	set_signal_heredoc(void);


#endif
