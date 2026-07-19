/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strchr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: taegokim <taegokim@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/13 17:13:17 by taegokim          #+#    #+#             */
/*   Updated: 2026/04/19 15:59:18 by taegokim         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// #include <stdio.h>
#include "libft.h"

/*
- Searches for the first occurrence of c in the string.
- It can also search for the terminating '\0' character.
- Returns NULL if c is not found.
*/
char	*ft_strchr(const char *s, int c)
{
	char	*p;

	p = (char *)s;
	while (1)
	{
		if (*p == (unsigned char)c)
			break ;
		else if (*p == '\0')
		{
			p = NULL;
			break ;
		}
		p++;
	}
	return (p);
}
