/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_tokenize.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tg <tg@student.42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/28 15:25:34 by taegokim          #+#    #+#             */
/*   Updated: 2026/09/13 11:47:27 by tg               ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lexer.h"
#include "libft.h"
#include "util.h"
#include <stddef.h>
#include <stdlib.h>

static char	*slice_token(const char **cursor)
{
	size_t	length;
	char	*value;

	if (cursor == NULL || *cursor == NULL || **cursor == '\0')
		return (NULL);
	length = lexer_token_length(*cursor);
	value = ft_substr(*cursor, 0, length);
	if (value == NULL)
		return (NULL);
	*cursor += length;
	return (value);
}

static t_status	append_token_list_value(t_token_list *token_list, char *value)
{
	t_token	*token;

	token = ft_calloc(1, sizeof(t_token));
	if (token == NULL)
		return (free(value), FAIL);
	if (token_init(token, lexer_classify_token_type(value), value) != OK)
		return (free(token), free(value), FAIL);
	if (token_list_add_token(token_list, token) != OK)
		return (token->destroy(token), free(token), FAIL);
	return (OK);
}

t_status	lexer_tokenize(const char *line, t_token_list *token_list)
{
	char		*value;
	const char	*cursor;

	if (line == NULL || token_list == NULL)
		return (FAIL);
	cursor = line;
	while (*cursor != '\0')
	{
		while (is_space(*cursor))
			cursor++;
		if (*cursor == '\0')
			break ;
		value = slice_token(&cursor);
		if (value == NULL)
			return (FAIL);
		if (append_token_list_value(token_list, value) != OK)
			return (FAIL);
	}
	return (OK);
}
