/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strmapi.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: taegokim <taegokim@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/18 13:58:38 by taegokim          #+#    #+#             */
/*   Updated: 2026/04/18 14:06:39 by taegokim         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strmapi(char const *s, char (*f)(unsigned int, char))
{
	size_t	s_len;
	size_t	i;
	char	*p;

	if (!s || !f)
		return (NULL);
	s_len = ft_strlen(s);
	p = (char *)malloc(s_len + 1);
	if (!p)
		return (NULL);
	i = 0;
	while (i < s_len)
	{
		p[i] = f(i, s[i]);
		i++;
	}
	p[s_len] = '\0';
	return (p);
}
