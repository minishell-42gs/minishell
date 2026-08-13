/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   is_same_str.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tg <tg@student.42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/13 12:43:49 by tg                #+#    #+#             */
/*   Updated: 2026/08/13 12:51:09 by tg               ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "util.h"
#include <stddef.h>

bool	is_same_str(const char *s1, const char *s2)
{
	int	len1;
	int	len2;

	if (s1 == NULL || s2 == NULL)
		return (false);
	len1 = ft_strlen(s1);
	len2 = ft_strlen(s2);
	if (len1 != len2 || ft_strncmp(s1, s2, len1) != 0)
		return (false);
	return (true);
}
