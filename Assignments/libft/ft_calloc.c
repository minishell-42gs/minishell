/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_calloc.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: taegokim <taegokim@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/14 18:27:41 by taegokim          #+#    #+#             */
/*   Updated: 2026/04/15 16:36:24 by taegokim         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// #include <stdio.h>
// #include <stdlib.h>
#include "libft.h"

void	*ft_calloc(size_t nmemb, size_t size)
{
	void	*p;

	if (size == 0 || nmemb == 0)
		return (malloc(0));
	if (SIZE_MAX / size < nmemb)
		return (NULL);
	p = malloc(nmemb * size);
	if (!p)
		return (NULL);
	ft_memset(p, 0, size * nmemb);
	return (p);
}

// int	main(void)
// {
// 	int	*i;
// 	char	*c;
// 	int	idx;

// 	i = (int *)ft_calloc(5, sizeof(int));
// 	if (!i)
// 		return (printf("allocation failed\n"), 1);
// 	printf("int array: ");
// 	idx = 0;
// 	while (idx < 5)
// 	{
// 		printf("%d ", i[idx]);
// 		idx++;
// 	}
// 	printf("\n");
// 	free(i);
// 	c = (char *)ft_calloc(6, sizeof(char));
// 	if (!c)
// 		return (printf("allocation failed\n"), 1);
// 	printf("first char code: %d\n", c[0]);
// 	free(c);
// 	return (0);
// }
