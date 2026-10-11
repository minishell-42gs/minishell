/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   is_blank.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tg <tg@student.42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/02 15:30:00 by hyuckwon          #+#    #+#             */
/*   Updated: 2026/09/13 11:48:11 by tg               ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "util.h"
#include <stddef.h>

bool	is_space(char c)
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
