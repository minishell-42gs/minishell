/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   is_blank.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hyuckwon <hyuckwon@student.42gyeongsan.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/02 15:30:00 by hyuckwon          #+#    #+#             */
/*   Updated: 2026/08/02 17:08:09 by hyuckwon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "util.h"
#include <stddef.h>

static bool	is_space(char c)
{
	return (c == ' ' || (c >= '\t' && c <= '\r'));
}

bool	is_blank(const char *str)
{
	int	i;

	if (str == NULL)
		return (true);
	i = 0;
	while (str[i] != '\0')
	{
		if (is_space(str[i]) == false)
			return (false);
		i++;
	}
	return (true);
}
