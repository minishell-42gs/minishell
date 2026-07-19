/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlcpy.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: taegokim <taegokim@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/09 17:40:22 by taegokim          #+#    #+#             */
/*   Updated: 2026/04/19 16:30:38 by taegokim         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

size_t	ft_strlcpy(char *dst, const char *src, size_t size)
{
	size_t			i;

	if (size == 0)
		return (ft_strlen(src));
	i = 0;
	while (i + 1 < size && src[i])
	{
		dst[i] = src[i];
		i++;
	}
	dst[i] = '\0';
	return (ft_strlen(src));
}
// #include <stdio.h>
// int	main(void)
// {
// 	char	dst1[10];
// 	char	dst2[5];
// 	size_t	r1;
// 	size_t	r2;

// 	r1 = strlcpy(dst1, "hello", sizeof(dst1));
// 	r2 = strlcpy(dst2, "hello", sizeof(dst2));
// 	printf("r1 = %zu, dst1 = [%s]\n", r1, dst1);
// 	printf("r2 = %zu, dst2 = [%s]\n", r2, dst2);
// }
