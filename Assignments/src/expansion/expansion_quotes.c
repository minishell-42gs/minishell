/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansion_quotes.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tg <tg@student.42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/04 15:00:00 by tg                #+#    #+#             */
/*   Updated: 2026/10/04 15:00:00 by tg               ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "expansion.h"
#include "libft.h"
#include <stdlib.h>

static size_t	copy_content(const char *raw, char *result)
{
	char	quote;
	size_t	i;
	size_t	out;

	i = 0;
	out = 0;
	quote = '\0';
	while (raw[i] != '\0')
	{
		if (quote == '\0' && (raw[i] == '\'' || raw[i] == '"'))
			quote = raw[i++];
		else if (raw[i] == quote)
		{
			quote = '\0';
			i++;
		}
		else
			result[out++] = raw[i++];
	}
	return (out);
}

char	*strip_quotes(const char *raw)
{
	char	*result;

	if (raw == NULL)
		return (NULL);
	result = ft_calloc(ft_strlen(raw) + 1, sizeof(char));
	if (result == NULL)
		return (NULL);
	copy_content(raw, result);
	return (result);
}
