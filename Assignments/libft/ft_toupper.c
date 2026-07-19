/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_toupper.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: taegokim <taegokim@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/13 16:47:47 by taegokim          #+#    #+#             */
/*   Updated: 2026/04/13 17:09:54 by taegokim         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// #include <stdio.h>
#include "libft.h"

int	ft_toupper(int c)
{
	if ('a' <= c && c <= 'z')
		c -= 'a' - 'A';
	return (c);
}

// int	main(void)
// {
// 	char	test[] = "Hello World 123!@#";
// 	int		i;

// 	printf("Original: %s\n", test);
// 	printf("Toupper:  ");
// 	i = 0;
// 	while (test[i])
// 	{
// 		printf("%c", ft_toupper(test[i]));
// 		i++;
// 	}
// 	printf("\n");
// 	printf("\nIndividual tests:\n");
// 	printf("ft_toupper('a') = %c (expect: A)\n", ft_toupper('a'));
// 	printf("ft_toupper('z') = %c (expect: Z)\n", ft_toupper('z'));
// 	printf("ft_toupper('A') = %c (expect: A)\n", ft_toupper('A'));
// 	printf("ft_toupper('Z') = %c (expect: Z)\n", ft_toupper('Z'));
// 	printf("ft_toupper('5') = %c (expect: 5)\n", ft_toupper('5'));
// 	printf("ft_toupper('!') = %c (expect: !)\n", ft_toupper('!'));
// 	return (0);
// }