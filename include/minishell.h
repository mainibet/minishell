/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: albetanc <albetanc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/21 10:47:36 by albetanc          #+#    #+#             */
/*   Updated: 2025/09/10 08:41:55 by albetanc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

// -----------------------------------------//
//              COMMON MACROS               //
// -----------------------------------------//

//to read from command line, from fd or from here_doc
#define BUFFER_SIZE 1024

// -----------------------------------------//
//        STANDARD HEADERS (COMMON)         //
// -----------------------------------------//

# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <sys/wait.h>
# include <readline/readline.h>
# include <readline/history.h>
# include <signal.h>
# include <sys/ioctl.h>
# include <termios.h>
# include <stdbool.h>

// -----------------------------------------//
//             MODULE HEADERS               //
// -----------------------------------------//

# include "../libft/libft.h"
# include "structs.h"
# include "colors.h"
# include "debug.h"
# include "cleanup.h"
# include "lexer.h"
# include "parser.h"
# include "prexec.h"
# include "exec.h"
# include "redir.h"
# include "builtin.h"
# include "expansion.h"
# include "export.h"
# include "unset.h"
# include "signall.h"
# include "heredoc.h"

// -----------------------------------------//
//                PROTOTYPES                //
// -----------------------------------------//

// --- PROGRAM INITIALIZATION --- //
void	init_program(t_program *program, char **envp);


#endif
