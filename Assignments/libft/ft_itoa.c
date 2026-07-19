/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: taegokim <taegokim@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/15 22:57:39 by taegokim          #+#    #+#             */
/*   Updated: 2026/04/16 12:59:57 by taegokim         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// #include <limits.h>
// #include <stdio.h>
// #include <string.h>
#include "libft.h"

static unsigned int	ft_abs(long long n)
{
	if (n >= 0)
		return ((unsigned int)n);
	else
		return ((unsigned int)(-n));
}

static void	slice_int2str(unsigned int n, size_t len, char *s)
{
	while (1)
	{
		s[--len] = (n % 10) + '0';
		n /= 10;
		if (n == 0)
			break ;
	}
}

static size_t	cnt_digit(int n)
{
	size_t	cnt;

	if (n == 0)
		return (1);
	cnt = 0;
	while (n != 0)
	{
		cnt++;
		n /= 10;
	}
	return (cnt);
}

char	*ft_itoa(int n)
{
	size_t	len;
	char	*p;

	len = cnt_digit((long long)n);
	if (n < 0)
		len += 1;
	p = (char *)malloc(len + 1);
	if (!p)
		return (NULL);
	slice_int2str(ft_abs(n), len, p);
	if (n < 0)
		p[0] = '-';
	p[len] = '\0';
	return (p);
}

// static int	check_itoa(int n)
// {
// 	char	*result;
// 	char	expected[32];

// 	result = ft_itoa(n);
// 	if (!result)
// 	{
// 		printf("FAIL: ft_itoa(%d) -> malloc failed\n", n);
// 		return (1);
// 	}
// 	snprintf(expected, sizeof(expected), "%d", n);
// 	if (strcmp(result, expected) != 0)
// 	{
// 		printf("FAIL: ft_itoa(%d) -> got [%s], expected [%s]\n",
// 			n, result, expected);
// 		free(result);
// 		return (1);
// 	}
// 	printf("OK:   ft_itoa(%d) -> [%s]\n", n, result);
// 	free(result);
// 	return (0);
// }

// int	main(void)
// {
// 	int	tests[] = {0, 1, -1, 9, -9, 10, -10, 42, -42,
// 					 1000, -1000, INT_MAX, INT_MIN};
// 	size_t	i;

// 	i = 0;
// 	while (i < sizeof(tests) / sizeof(tests[0]))
// 	{
// 		if (check_itoa(tests[i]) != 0)
// 			return (1);
// 		i++;
// 	}
// 	return (0);
// }
