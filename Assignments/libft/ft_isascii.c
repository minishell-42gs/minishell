/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_isascii.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: taegokim <taegokim@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/09 17:40:53 by taegokim          #+#    #+#             */
/*   Updated: 2026/04/09 18:08:16 by taegokim         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// #include <stdio.h>
#include "libft.h"

int	ft_isascii(int c)
{
	if (0 <= c && c <= 127)
		return (1);
	else
		return (0);
}

// int main(){
// 	printf("%d\n", ft_isascii('1'));
// 	printf("%d\n", ft_isascii('a'));
// 	printf("%d\n", ft_isascii('%'));
// 	printf("%d\n", ft_isascii('9'));
// 	printf("%d\n", ft_isascii('Q'));
// 	printf("%d\n", ft_isascii(127));
// 	printf("%d\n", ft_isascii(0));
// 	printf("%d\n", ft_isascii(128));
// }