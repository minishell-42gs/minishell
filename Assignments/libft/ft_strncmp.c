/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strncmp.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: taegokim <taegokim@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/14 15:41:23 by taegokim          #+#    #+#             */
/*   Updated: 2026/04/14 16:03:45 by taegokim         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// #include <stdio.h>
// #include <string.h>
#include "libft.h"

// Compares up to n characters, including the '\0' terminator when reached.
int	ft_strncmp(const char *s1, const char *s2, size_t n)
{
	size_t	i;

	i = 0;
	while (i < n)
	{
		if (s1[i] != s2[i] || s1[i] == '\0' || s2[i] == '\0')
			return ((unsigned char)s1[i] - (unsigned char)s2[i]);
		i++;
	}
	return (0);
}

// int	main(void)
// {
// 	printf("ft_strncmp(\"abc\", \"abc\", 3) = %d | strncmp = %d\n",
// 		ft_strncmp("abc", "abc", 3), strncmp("abc", "abc", 3));
// 	printf("ft_strncmp(\"abc\", \"abd\", 3) = %d | strncmp = %d\n",
// 		ft_strncmp("abc", "abd", 3), strncmp("abc", "abd", 3));
// 	printf("ft_strncmp(\"abc\", \"abd\", 2) = %d | strncmp = %d\n",
// 		ft_strncmp("abc", "abd", 2), strncmp("abc", "abd", 2));
// 	printf("ft_strncmp(\"abc\", \"ab\", 3) = %d | strncmp = %d\n",
// 		ft_strncmp("abc", "ab", 3), strncmp("abc", "ab", 3));
// 	printf("ft_strncmp(\"\", \"\", 0) = %d | strncmp = %d\n",
// 		ft_strncmp("", "", 0), strncmp("", "", 0));
// 	return (0);
// }