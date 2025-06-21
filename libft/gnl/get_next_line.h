/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.h                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: albetanc <albetanc@student.42berlin.d      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/23 15:29:36 by albetanc          #+#    #+#             */
/*   Updated: 2025/01/24 18:25:55 by albetanc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef GET_NEXT_LINE_H
# define GET_NEXT_LINE_H

# ifndef BUFFER_SIZE
#  define BUFFER_SIZE 1024
# endif

# include <stdlib.h>
# include <unistd.h>
# include <stddef.h>

char	*get_next_line(int fd);
size_t	ft_str_utils(char *dst, const char *src, size_t dstsize, int flag);
char	*ft_strchr(const char *str, int c);
size_t	ft_strlcat(char *dst, const char *src, size_t dst_size);
char	*ft_strjoin(char const *s1, char const *s2);
char	*ft_process_str(char *join, char *c, int flag);

#endif
