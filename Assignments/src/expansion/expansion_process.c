/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansion_process.c                                :+:      :+:    :+:   */
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

typedef struct s_word_context
{
	const char			*raw;
	size_t				*index;
	char				*quote;
	t_expansion_context	*expansion;
	t_word_state		*word;
}	t_word_context;

static t_status	append_variable(t_word_context *state)
{
	char		*value;
	t_status	status;

	value = exp_variable_value(state->raw, state->index, state->expansion);
	if (value == NULL)
		return (FAIL);
	status = exp_append_text(state->word, value, *state->quote == '\0');
	free(value);
	return (status);
}

static int	update_quote(char c, char *quote, t_word_state *word)
{
	if (*quote == '\0' && (c == '\'' || c == '"'))
	{
		*quote = c;
		word->active = true;
		return (1);
	}
	if (c == *quote)
	{
		*quote = '\0';
		return (1);
	}
	return (0);
}

static t_status	process_char(t_word_context *state)
{
	if (update_quote(state->raw[*state->index], state->quote, state->word))
	{
		(*state->index)++;
		return (OK);
	}
	if (state->raw[*state->index] == '$' && *state->quote != '\'')
		return (append_variable(state));
	return (exp_append_char(state->word,
			state->raw[(*state->index)++]));
}

t_status	exp_process_word(const char *raw, t_expansion_context *expansion,
		char ***fields, size_t *count)
{
	t_word_state	word;
	t_word_context	state;
	char			quote;
	size_t			i;

	word = (t_word_state){0};
	quote = '\0';
	i = 0;
	state = (t_word_context){raw, &i, &quote, expansion, &word};
	while (raw[i] != '\0')
	{
		if (process_char(&state) != OK)
			return (free(word.value), free_split(word.fields), FAIL);
	}
	if (exp_append_field(&word) != OK)
		return (free(word.value), free_split(word.fields), FAIL);
	*fields = word.fields;
	*count = word.count;
	return (OK);
}

t_status	exp_append_fields(t_cmd *cmd, char **fields, size_t count)
{
	size_t	i;

	i = 0;
	while (i < count)
	{
		if (cmd_append_argv(cmd, fields[i]) != OK)
			return (FAIL);
		fields[i] = NULL;
		i++;
	}
	return (OK);
}
