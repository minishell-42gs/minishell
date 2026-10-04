/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_token_helpers.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tg <tg@student.42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/04 15:00:00 by tg                #+#    #+#             */
/*   Updated: 2026/10/04 15:00:00 by tg               ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lexer.h"
#include "util.h"
#include <stddef.h>

static size_t	operator_length(const char *cursor)
{
	if (cursor[0] == '<' || cursor[0] == '>')
	{
		if (cursor[1] == cursor[0])
			return (2);
		return (1);
	}
	return (0);
}

size_t	lexer_token_length(const char *cursor)
{
	size_t	length;
	char	quote;

	length = operator_length(cursor);
	if (length > 0)
		return (length);
	if (cursor[0] == '|' && cursor[1] != '|')
		return (1);
	quote = '\0';
	length = 0;
	while (cursor[length] != '\0')
	{
		if (quote == '\0' && (cursor[length] == '\''
				|| cursor[length] == '"'))
			quote = cursor[length];
		else if (quote == cursor[length])
			quote = '\0';
		else if (quote == '\0' && (is_space(cursor[length])
				|| operator_length(cursor + length) > 0
				|| (cursor[length] == '|' && cursor[length + 1] != '|'
					&& (length == 0 || cursor[length - 1] != '|'))))
			break ;
		length++;
	}
	return (length);
}

t_token_type	lexer_classify_token_type(const char *str)
{
	if (str[0] == '|' && str[1] == '\0')
		return (TOKEN_PIPE);
	if (str[0] == '<' && str[1] == '<')
		return (TOKEN_HEREDOC);
	if (str[0] == '>' && str[1] == '>')
		return (TOKEN_REDIR_APPEND);
	if (str[0] == '<')
		return (TOKEN_REDIR_IN);
	if (str[0] == '>')
		return (TOKEN_REDIR_OUT);
	return (TOKEN_WORD);
}
