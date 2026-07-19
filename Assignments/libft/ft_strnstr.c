/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strnstr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: taegokim <taegokim@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/14 16:54:18 by taegokim          #+#    #+#             */
/*   Updated: 2026/04/14 17:41:57 by taegokim         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// #include <stdio.h>
#include "libft.h"

char	*ft_strnstr(const char *big, const char *little, size_t len)
{
	size_t	i;
	size_t	j;

	if (little[0] == '\0')
		return ((char *)big);
	i = 0;
	while (big[i] != '\0' && i < len)
	{
		j = 0;
		while (big[i + j] != '\0' && i + j < len && little[j] == big[i + j])
		{
			if (little[j + 1] == '\0')
				return ((char *)(big + i));
			j++;
		}
		i++;
	}
	return (NULL);
}

// int	main(void)
// {
// 	char	*res;

// 	res = ft_strnstr("Hello World", "World", 11);
// 	printf("1) %s\n", res ? res : "NULL");
// 	res = ft_strnstr("Hello World", "World", 8);
// 	printf("2) %s\n", res ? res : "NULL");
// 	res = ft_strnstr("abc", "", 3);
// 	printf("3) %s\n", res ? res : "NULL");
// 	return (0);
// }