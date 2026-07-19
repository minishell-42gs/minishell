/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlcat.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: taegokim <taegokim@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/13 15:26:48 by taegokim          #+#    #+#             */
/*   Updated: 2026/04/19 16:44:51 by taegokim         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// #include <stdio.h>
#include "libft.h"

size_t	ft_strlcat(char *dst, const char *src, size_t size)
{
	size_t	i;
	size_t	dst_len;
	size_t	src_len;

	src_len = ft_strlen(src);
	dst_len = 0;
	while (dst_len < size && dst[dst_len] != '\0')
		dst_len++;
	if (dst_len == size)
		return (src_len + size);
	i = 0;
	while (dst_len + i + 1 < size && src[i] != '\0')
	{
		dst[dst_len + i] = src[i];
		i++;
	}
	dst[dst_len + i] = '\0';
	return (src_len + dst_len);
}
// int	main(void)
// {
// 	char buf1[20] = "Hello";
// 	char buf2[10] = "Hello";
// 	char buf3[6] = "Hello";
// 	size_t ret1;
// 	size_t ret2;
// 	size_t ret3;

// 	ret1 = ft_strlcat(buf1, " World", sizeof(buf1));
// 	ret2 = ft_strlcat(buf2, " World", sizeof(buf2));
// 	ret3 = ft_strlcat(buf3, " World", sizeof(buf3));
// 	printf("ret1 = %zu, buf1 = [%s]\n", ret1, buf1);
// 	printf("ret2 = %zu, buf2 = [%s]\n", ret2, buf2);
// 	printf("ret3 = %zu, buf3 = [%s]\n", ret3, buf3);
// 	printf("truncated buf2? %s\n", (ret2 >= sizeof(buf2)) ? "yes" : "no");
// 	printf("truncated buf3? %s\n", (ret3 >= sizeof(buf3)) ? "yes" : "no");
// 	return (0);
// }
