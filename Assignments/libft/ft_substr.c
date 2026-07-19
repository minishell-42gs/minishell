/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_substr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: taegokim <taegokim@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/15 14:18:14 by taegokim          #+#    #+#             */
/*   Updated: 2026/04/15 16:43:31 by taegokim         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
// #include <stdio.h>

static size_t	calc_min(size_t a, size_t b)
{
	if (a < b)
		return (a);
	else
		return (b);
}

char	*ft_substr(char const *s, unsigned int start, size_t len)
{
	size_t	i;
	size_t	s_len;
	size_t	m_size;
	char	*ret;

	s_len = ft_strlen(s);
	if (start < s_len)
		m_size = calc_min(s_len - start, len);
	else
		m_size = 0;
	ret = (char *)malloc(m_size + 1);
	if (!ret)
		return (NULL);
	i = 0;
	while (i < m_size)
	{
		ret[i] = s[start + i];
		i++;
	}
	ret[i] = '\0';
	return (ret);
}

// int	main(void)
// {
// 	char	*result;

// 	/* 일반 케이스 */
// 	result = ft_substr("Hello, World!", 7, 5);
// 	printf("[1] ft_substr(\"Hello, World!\", 7, 5) = \"%s\"\n", result);
// 	printf("    기대값: \"World\"\n");
// 	free(result);

// 	/* start가 0인 경우 */
// 	result = ft_substr("Hello", 0, 3);
// 	printf("[2] ft_substr(\"Hello\", 0, 3) = \"%s\"\n", result);
// 	printf("    기대값: \"Hel\"\n");
// 	free(result);

// 	/* len이 문자열보다 긴 경우 */
// 	result = ft_substr("Hi", 0, 100);
// 	printf("[3] ft_substr(\"Hi\", 0, 100) = \"%s\"\n", result);
// 	printf("    기대값: \"Hi\"\n");
// 	free(result);

// 	/* start가 문자열 길이와 같은 경우 */
// 	result = ft_substr("Hello", 5, 3);
// 	printf("[4] ft_substr(\"Hello\", 5, 3) = \"%s\"\n", result);
// 	printf("    기대값: \"\"\n");
// 	free(result);

// 	/* start가 문자열 길이를 초과하는 경우 */
// 	result = ft_substr("Hello", 10, 3);
// 	printf("[5] ft_substr(\"Hello\", 10, 3) = \"%s\"\n", result);
// 	printf("    기대값: \"\"\n");
// 	free(result);

// 	/* len이 0인 경우 */
// 	result = ft_substr("Hello", 2, 0);
// 	printf("[6] ft_substr(\"Hello\", 2, 0) = \"%s\"\n", result);
// 	printf("    기대값: \"\"\n");
// 	free(result);

// 	/* 빈 문자열 */
// 	result = ft_substr("", 0, 5);
// 	printf("[7] ft_substr(\"\", 0, 5) = \"%s\"\n", result);
// 	printf("    기대값: \"\"\n");
// 	free(result);

// 	return (0);
// }
