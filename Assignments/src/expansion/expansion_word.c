/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansion_word.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tg <tg@student.42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/04 15:00:00 by tg                #+#    #+#             */
/*   Updated: 2026/10/04 15:00:00 by tg               ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "expansion_internal.h"
#include "libft.h"
#include "util.h"
#include <stdlib.h>

static t_status	reserve_value(t_word_state *state)
{
	char	*next;
	size_t	capacity;
	size_t	i;

	if (state->length + 1 < state->capacity)
		return (OK);
	capacity = state->capacity * 2;
	if (capacity < 16)
		capacity = 16;
	next = ft_calloc(capacity, sizeof(char));
	if (next == NULL)
		return (FAIL);
	i = 0;
	while (i < state->length)
	{
		next[i] = state->value[i];
		i++;
	}
	free(state->value);
	state->value = next;
	state->capacity = capacity;
	return (OK);
}

t_status	exp_append_char(t_word_state *state, char c)
{
	if (reserve_value(state) != OK)
		return (FAIL);
	state->value[state->length++] = c;
	state->value[state->length] = '\0';
	state->active = true;
	return (OK);
}

static char	**create_fields(t_word_state *state)
{
	char	**next;
	size_t	i;

	next = ft_calloc(state->count + 2, sizeof(char *));
	if (next == NULL)
		return (NULL);
	i = 0;
	while (i < state->count)
	{
		next[i] = state->fields[i];
		i++;
	}
	next[state->count] = state->value;
	return (next);
}

t_status	exp_append_field(t_word_state *state)
{
	char	**next;

	if (!state->active)
		return (OK);
	if (state->value == NULL)
		state->value = ft_calloc(1, sizeof(char));
	if (state->value == NULL)
		return (FAIL);
	next = create_fields(state);
	if (next == NULL)
		return (FAIL);
	state->count++;
	free(state->fields);
	state->fields = next;
	state->value = NULL;
	state->length = 0;
	state->capacity = 0;
	state->active = false;
	return (OK);
}

t_status	exp_append_text(t_word_state *state, const char *text, bool split)
{
	size_t	i;

	i = 0;
	while (text != NULL && text[i] != '\0')
	{
		if (split && is_space(text[i]))
		{
			if (exp_append_field(state) != OK)
				return (FAIL);
		}
		else if (exp_append_char(state, text[i]) != OK)
			return (FAIL);
		i++;
	}
	return (OK);
}
