/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_tokenize.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: taegokim <taegokim@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/28 15:25:34 by taegokim          #+#    #+#             */
/*   Updated: 2026/08/31 20:53:48 by taegokim         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lexer.h"
#include "libft.h"
#include <stddef.h>
#include <stdlib.h>

static t_token_type	classify_token_type(const char *str)
{
	if (str[0] == '|' && str[1] == '\0')
		return (TOKEN_PIPE);
	return (TOKEN_WORD);
}

static size_t	token_length(const char *cursor)
{
	size_t	length;
	char	quote;

	if (cursor[0] == '|' && cursor[1] != '|')
		return (1);
	quote = '\0';
	length = 0;
	while (cursor[length] != '\0')
	{
		if (quote == '\0'
			&& (cursor[length] == '\'' || cursor[length] == '"'))
			quote = cursor[length];
		else if (quote == cursor[length])
			quote = '\0';
		else if (quote == '\0' && (cursor[length] == ' '
				|| (cursor[length] == '|' && cursor[length + 1] != '|')))
			break ;
		if (quote == '\0' && cursor[length] == '|')
			length++;
		length++;
	}
	return (length);
}

static char	*slice_token(const char **cursor)
{
	size_t	length;
	char	*value;

	if (cursor == NULL || *cursor == NULL || **cursor == '\0')
		return (NULL);
	length = token_length(*cursor);
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
	if (token_init(token, classify_token_type(value), value) != OK)
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
		while (*cursor == ' ')
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
