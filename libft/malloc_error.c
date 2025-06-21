/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   malloc_error.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: albetanc <albetanc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/02 13:54:01 by albetanc          #+#    #+#             */
/*   Updated: 2025/05/23 17:16:48 by albetanc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

// char	*malloc_error(void)
// {
// 	perror("Malloc error");
// 	return (NULL);
// }
char	*malloc_error(void)
{
	safe_write(STDERR_FILENO, "Error\n", 6);
	exit(1);
}
