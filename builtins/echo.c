/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: albetanc <albetanc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/23 14:29:17 by albetanc          #+#    #+#             */
/*   Updated: 2025/06/23 16:47:34 by albetanc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

//echo - display a line of text
//v0: no flags
//v1: echo -n
//-e enable interpretation of backslash escapes
// - Execute the echo command with or without arguments or -n
// - Repeat multiple times with different arguments
// - if something crashes use the crash flag.
// - if something is not working use the incomplete work flag.