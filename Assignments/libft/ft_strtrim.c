/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strtrim.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: taegokim <taegokim@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/15 17:13:10 by taegokim          #+#    #+#             */
/*   Updated: 2026/04/15 21:41:08 by taegokim         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// #include <stdio.h>
#include "libft.h"

static size_t	find_start(char const *s1, char const *set)
{
	size_t	i;

	i = 0;
	while (s1[i] != '\0')
	{
		if (!ft_strchr(set, s1[i]))
			break ;
		i++;
	}
	return (i);
}

static size_t	find_end(char const *s1, char const *set)
{
	size_t	i;

	i = ft_strlen(s1);
	while (i > 0)
	{
		if (!ft_strchr(set, s1[i - 1]))
			break ;
		i--;
	}
	return (i);
}

char	*ft_strtrim(char const *s1, char const *set)
{
	size_t	start;
	size_t	end;
	size_t	slen;
	char	*p;

	start = find_start(s1, set);
	end = find_end(s1, set);
	if (start < end)
		slen = end - start;
	else
		slen = 0;
	p = (char *)malloc(slen + 1);
	if (!p)
		return (NULL);
	ft_memcpy(p, s1 + start, slen);
	p[slen] = '\0';
	return (p);
}

// int	main(void)
// {
// 	char	*result;
// 	char	*input[] = {"   hello world   ", "xx42GSxx",
// 			"\n\ttrim me\t\n", "aaaa", "", "no_trim"};
// 	char	*set[] = {" ", "x", "\n\t", "a", " ", "xyz"};
// 	int		i;

// 	i = 0;
// 	while (i < 6)
// 	{
// 		result = ft_strtrim(input[i], set[i]);
// 		if (!result)
// 		{
// 			printf("[%d] malloc failed\n", i + 1);
// 			return (1);
// 		}
// 		printf("[%d] input: \"%s\" | set: \"%s\" | output: \"%s\"\n", i + 1,
// 			input[i], set[i], result);
// 		free(result);
// 		i++;
// 	}
// 	return (0);
// }
