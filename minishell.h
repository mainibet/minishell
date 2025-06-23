/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: albetanc <albetanc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/21 10:47:36 by albetanc          #+#    #+#             */
/*   Updated: 2025/06/23 09:49:49 by albetanc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

// -----------------------------------------//
//              COMMON MACROS               //
// -----------------------------------------//

//include them here if needed 

// -----------------------------------------//
//        STANDARD HEADERS (COMMON)         //
// -----------------------------------------//
//STDIO.H FOR: printf, perror
//STDLIB.H FOR: malloc, free, exit, getenv
//UNISTD.H FOR: read, write, access, execve and others
//may be here also later: sys/wait, sys/types: size_t, pid_t, strings.h
# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>

// -------Readline specific headers
#include <readline/readline.h>
#include <readline/history.h>

// -------Execution specific headers
//sys/wait: waitpid, wait, wait3, wait4
# include <sys/wait.h>

// -----------------------------------------//
//             MODULE HEADERS               //
// -----------------------------------------//
//The modular headers include the prototypes from the 
//functions for that part of the program
# include "libft/libft.h"
# include "exec/exec.h"

#endif