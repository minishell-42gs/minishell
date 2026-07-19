/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memmove.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: taegokim <taegokim@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/09 15:44:05 by taegokim          #+#    #+#             */
/*   Updated: 2026/04/14 16:30:57 by taegokim         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// #include <stdio.h>
/*
Unlike memcpy, this function is safe when memory regions overlap.
If dest and src overlap, copying can overwrite source bytes during the move.
ft_memmove handles this case by choosing a safe copy direction.
*/
#include "libft.h"

void	*ft_memmove(void *dest, const void *src, size_t n)
{
	unsigned char	*d;
	unsigned char	*s;

	d = (unsigned char *)dest;
	s = (unsigned char *)src;
	if (s < d)
		while (n--)
			d[n] = s[n];
	else if (d < s)
		while (n--)
			*d++ = *s++;
	return (dest);
}

// int main(){
// 	char str[30] = "hello world~";
// 	ft_memmove(str + 3, str, sizeof(str));
// 	printf("%s\n", str);
// }