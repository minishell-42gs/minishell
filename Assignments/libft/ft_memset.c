/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memset.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: taegokim <taegokim@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/09 15:36:50 by taegokim          #+#    #+#             */
/*   Updated: 2026/04/13 14:26:04 by taegokim         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// #include <stdio.h>
#include "libft.h"

void	*ft_memset(void *s, int c, size_t n)
{
	unsigned char	*p;

	p = (unsigned char *)s;
	while (n--)
	{
		*p++ = (unsigned char)c;
	}
	return (s);
}

// int main(){
// 	char s[10] = "hello";
// 	printf("%s\n", s);

// 	ft_memset(s, 1, sizeof(s));

// 	for(int i = 0; i<10; i++){
// 		printf("%d ", s[i]);
// 	}

// 	ft_memset(s, 'A', sizeof(s));
// 	printf("%s\n", s);
// }