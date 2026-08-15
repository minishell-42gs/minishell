/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   util.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tg <tg@student.42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/19 13:55:37 by taegokim          #+#    #+#             */
/*   Updated: 2026/08/13 12:47:16 by tg               ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef UTIL_H
# define UTIL_H

# include <stdbool.h>

void	free_split(char **split);
bool	is_blank(const char *str);
bool	is_same_str(const char *s1, const char *s2);

#endif // UTIL_H