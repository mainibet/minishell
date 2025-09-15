/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   malloc_error.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: albetanc <albetanc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/02 13:54:01 by albetanc          #+#    #+#             */
/*   Updated: 2025/09/15 14:55:20 by albetanc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	malloc_error(t_program *program)
{
	safe_write(STDERR_FILENO, "Error\n", 6);
	cleanup_program(program); 
}
