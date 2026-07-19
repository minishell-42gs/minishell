/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memcmp.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: taegokim <taegokim@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/14 16:33:48 by taegokim          #+#    #+#             */
/*   Updated: 2026/04/14 16:43:07 by taegokim         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// #include <stdio.h>
// #include <string.h>
#include "libft.h"

int	ft_memcmp(const void *s1, const void *s2, size_t n)
{
	size_t	i;

	i = 0;
	while (i < n)
	{
		if (((unsigned char *)s1)[i] != ((unsigned char *)s2)[i])
			return (((unsigned char *)s1)[i] - ((unsigned char *)s2)[i]);
		i++;
	}
	return (0);
}

// int	main(void)
// {
// 	char	a[] = "abcdef";
// 	char	b[] = "abcdef";
// 	char	c[] = "abcdeg";
// 	unsigned char	x[] = {0, 1, 2, 3, 255};
// 	unsigned char	y[] = {0, 1, 2, 4, 255};

// 	printf("ft_memcmp(a, b, 6) = %d | memcmp = %d\n",
// 		ft_memcmp(a, b, 6), memcmp(a, b, 6));
// 	printf("ft_memcmp(a, c, 6) = %d | memcmp = %d\n",
// 		ft_memcmp(a, c, 6), memcmp(a, c, 6));
// 	printf("ft_memcmp(a, c, 5) = %d | memcmp = %d\n",
// 		ft_memcmp(a, c, 5), memcmp(a, c, 5));
// 	printf("ft_memcmp(x, y, 5) = %d | memcmp = %d\n",
// 		ft_memcmp(x, y, 5), memcmp(x, y, 5));
// 	printf("ft_memcmp(a, c, 0) = %d | memcmp = %d\n",
// 		ft_memcmp(a, c, 0), memcmp(a, c, 0));
// 	return (0);
// }