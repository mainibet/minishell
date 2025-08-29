/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   debug.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: albetanc <albetanc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/29 09:25:34 by albetanc          #+#    #+#             */
/*   Updated: 2025/08/29 09:48:48 by albetanc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef DEBUG_H
#define DEBUG_H

#ifdef DEBUG
# define DEBUG_PRINT(format, ...) fprintf(stderr, format, ##__VA_ARGS__)
#else
# define DEBUG_PRINT(format, ...) do {} while (0)
#endif

#define DEBUG_ERROR(format, ...) fprintf(stderr, format, ##__VA_ARGS__)

#endif