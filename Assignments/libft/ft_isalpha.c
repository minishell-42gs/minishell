/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_isalpha.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: taegokim <taegokim@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/09 17:40:48 by taegokim          #+#    #+#             */
/*   Updated: 2026/04/13 14:19:04 by taegokim         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// #include <stdio.h>

#include "libft.h"

int	ft_isalpha(int c)
{
	if (('a' <= c && c <= 'z') || ('A' <= c && c <= 'Z'))
	{
		return (1);
	}
	else
	{
		return (0);
	}
}

// int main(){
// 	printf("%d\n", ft_isalpha('1'));
// 	printf("%d\n", ft_isalpha('a'));
// 	printf("%d\n", ft_isalpha('Y'));
// 	printf("%d\n", ft_isalpha('%'));
// }