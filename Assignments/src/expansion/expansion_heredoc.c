/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansion_heredoc.c                                :+:      :+:    :+:   */
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

static t_status	append_heredoc_variable(const char *raw, size_t *index,
		t_expansion_context *context, t_word_state *word)
{
	char	*value;

	value = exp_variable_value(raw, index, context);
	if (value == NULL)
		return (FAIL);
	if (exp_append_text(word, value, false) != OK)
		return (free(value), FAIL);
	free(value);
	return (OK);
}

char	*expansion_heredoc_line(const char *raw, t_env_list *env,
		int status, bool expand)
{
	t_word_state		word;
	t_expansion_context	context;
	size_t				i;

	word = (t_word_state){0};
	context.env_list = env;
	context.last_status = status;
	i = 0;
	while (raw[i] != '\0')
	{
		if (expand && raw[i] == '$')
		{
			if (append_heredoc_variable(raw, &i, &context, &word) != OK)
				return (free(word.value), NULL);
		}
		else if (exp_append_char(&word, raw[i++]) != OK)
			return (free(word.value), NULL);
	}
	if (word.value == NULL)
		word.value = ft_calloc(1, sizeof(char));
	return (word.value);
}
