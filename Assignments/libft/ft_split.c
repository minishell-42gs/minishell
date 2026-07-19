/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: taegokim <taegokim@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/16 16:15:17 by taegokim          #+#    #+#             */
/*   Updated: 2026/04/18 13:48:53 by taegokim         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// #include <stdio.h>
#include "libft.h"

static size_t	cnt_words(char const *s, char c)
{
	size_t	ret;
	int		flag;
	size_t	i;

	i = 0;
	ret = 0;
	flag = 1;
	while (s[i] != '\0')
	{
		if (s[i] == c)
			flag = 1;
		else
		{
			if (flag)
			{
				ret++;
				flag = 0;
			}
		}
		i++;
	}
	return (ret);
}

static size_t	find_next_start(char const *s, size_t start, char c)
{
	size_t	next_idx;

	next_idx = start;
	while (s[next_idx] != '\0' && s[next_idx] == c)
		next_idx++;
	return (next_idx);
}

static size_t	cnt_word_len(char const *s, size_t start, char c)
{
	size_t	ret;

	ret = 0;
	while (s[start + ret] != '\0' && s[start + ret] != c)
		ret++;
	return (ret);
}

static int	slicing(char const *s, char c, char **ret)
{
	size_t	i;
	size_t	s_len;
	size_t	w_len;
	size_t	w_cnt;
	char	*p_word;

	s_len = ft_strlen(s);
	i = find_next_start(s, 0, c);
	w_cnt = 0;
	while (i < s_len)
	{
		w_len = cnt_word_len(s, i, c);
		p_word = (char *)malloc(w_len + 1);
		if (!p_word)
		{
			while (w_cnt > 0)
				free(ret[--w_cnt]);
			return (-1);
		}
		ft_memcpy(p_word, s + i, w_len);
		p_word[w_len] = '\0';
		ret[w_cnt++] = p_word;
		i = find_next_start(s, i + w_len, c);
	}
	return (0);
}

char	**ft_split(char const *s, char c)
{
	char	**ret;
	size_t	word_cnt;

	if (s == NULL)
		return (NULL);
	word_cnt = cnt_words(s, c);
	ret = (char **)malloc((word_cnt + 1) * sizeof(char *));
	if (!ret)
		return (NULL);
	if (slicing(s, c, ret) == -1)
	{
		free(ret);
		return (NULL);
	}
	ret[word_cnt] = NULL;
	return (ret);
}
// int	main(void)
// {
// 	char		*tests[] = {
// 		"hello world from libft",
// 		"   leading   and   trailing   ",
// 		"a,b,c,d",
// 		",,,a,,,b,,,",
// 		"nodelem",
// 		""
// 	};
// 	char		delims[] = {' ', ' ', ',', ',', ',', ' '};
// 	char		**res;
// 	size_t	t;
// 	size_t	i;

// 	t = 0;
// 	while (t < sizeof(tests) / sizeof(tests[0]))
// 	{
// 		printf("input: \"%s\" / delim: '%c'\n", tests[t], delims[t]);
// 		res = ft_split(tests[t], delims[t]);
// 		if (!res)
// 		{
// 			printf("ft_split returned NULL\n\n");
// 			t++;
// 			continue ;
// 		}
// 		i = 0;
// 		while (res[i])
// 		{
// 			printf("[%zu] %s\n", i, res[i]);
// 			free(res[i]);
// 			i++;
// 		}
// 		printf("count: %zu\n\n", i);
// 		free(res);
// 		t++;
// 	}
// 	return (0);
// }
