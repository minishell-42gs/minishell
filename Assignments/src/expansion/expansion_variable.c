/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansion_variable.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tg <tg@student.42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/04 15:00:00 by tg                #+#    #+#             */
/*   Updated: 2026/10/04 15:00:00 by tg               ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "expansion_internal.h"
#include "libft.h"
#include <stdlib.h>

bool	is_name_start(char c)
{
	return (ft_isalpha(c) || c == '_');
}

bool	is_name_char(char c)
{
	return (ft_isalnum(c) || c == '_');
}

static char	*lookup_value(const char *raw, size_t *index,
		t_expansion_context *context)
{
	char	*key;
	char	*value;
	size_t	start;

	start = *index;
	while (is_name_char(raw[*index]))
		(*index)++;
	key = ft_substr(raw, start, *index - start);
	if (key == NULL)
		return (NULL);
	value = context->env_list->get(context->env_list, key);
	free(key);
	if (value == NULL)
		return (ft_strdup(""));
	return (ft_strdup(value));
}

char	*exp_variable_value(const char *raw, size_t *index,
		t_expansion_context *context)
{
	(*index)++;
	if (raw[*index] == '?')
	{
		(*index)++;
		return (ft_itoa(context->last_status));
	}
	if (!is_name_start(raw[*index]))
		return (ft_strdup("$"));
	return (lookup_value(raw, index, context));
}
