/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memchr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: taegokim <taegokim@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/14 16:05:17 by taegokim          #+#    #+#             */
/*   Updated: 2026/04/14 16:28:25 by taegokim         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// #include <stdio.h>
// #include <string.h>
#include "libft.h"

/*
** Searches for the first occurrence of the byte c in the memory area s
** of size n. Returns a pointer to the matching byte, or NULL if not found.
** Unlike strchr, stops only at n bytes (not at '\0').
*/
void	*ft_memchr(const void *s, int c, size_t n)
{
	size_t	i;

	i = 0;
	while (i < n)
	{
		if (((unsigned char *)s)[i] == (unsigned char)c)
			return ((unsigned char *)s + i);
		i++;
	}
	return (NULL);
}

// int	main(void)
// {
// 	char	str[] = "Hello World";

// 	printf("=== Basic Search ===\n");
// 	printf("memchr(\"Hello World\", 'W', 11) found at: %s | ft_memchr: %s\n",
// 		(char *)memchr(str, 'W', 11), (char *)ft_memchr(str, 'W', 11));
// 	printf("memchr(\"Hello World\", 'o', 11) found at: %s | ft_memchr: %s\n",
// 		(char *)memchr(str, 'o', 11), (char *)ft_memchr(str, 'o', 11));

// 	printf("\n=== Range Limited Search ===\n");
// 	printf("memchr(\"Hello World\", 'W', 5) = %p | ft_memchr = %p\n",
// 		memchr(str, 'W', 5), ft_memchr(str, 'W', 5));
// 	printf("memchr(\"Hello World\", 'o', 5) found at: %s | ft_memchr: %s\n",
// 		(char *)memchr(str, 'o', 5), (char *)ft_memchr(str, 'o', 5));

// 	printf("\n=== Not Found ===\n");
// 	printf("memchr(\"Hello World\", 'z', 11) = %p | ft_memchr = %p\n",
// 		memchr(str, 'z', 11), ft_memchr(str, 'z', 11));

// 	printf("\n=== Zero Range ===\n");
// 	printf("memchr(\"Hello World\", 'H', 0) = %p | ft_memchr = %p\n",
// 		memchr(str, 'H', 0), ft_memchr(str, 'H', 0));

// 	return (0);
// }
