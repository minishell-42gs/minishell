/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_isdigit.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: taegokim <taegokim@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/09 17:40:56 by taegokim          #+#    #+#             */
/*   Updated: 2026/04/13 14:20:10 by taegokim         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// #include <stdio.h>
#include "libft.h"

int	ft_isdigit(int c)
{
	if ('0' <= c && c <= '9')
		return (1);
	else
		return (0);
}

// int main(){
// 	printf("%d\n", isdigit('1'));
// 	printf("%d\n", isdigit('a'));
// 	printf("%d\n", isdigit('%'));
// 	printf("%d\n", isdigit('9'));
// }