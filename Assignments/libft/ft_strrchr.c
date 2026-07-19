/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strrchr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: taegokim <taegokim@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/13 17:56:36 by taegokim          #+#    #+#             */
/*   Updated: 2026/04/19 15:58:34 by taegokim         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// #include <stdio.h>
#include "libft.h"

char	*ft_strrchr(const char *s, int c)
{
	char	*p;
	char	*ret;

	p = (char *)s;
	ret = NULL;
	while (1)
	{
		if (*p == (unsigned char)c)
			ret = p;
		if (*p == '\0')
			break ;
		p++;
	}
	return (ret);
}
