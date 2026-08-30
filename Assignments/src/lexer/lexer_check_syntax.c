/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_check_syntax.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tg <tg@student.42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/30 15:06:06 by tg                #+#    #+#             */
/*   Updated: 2026/08/30 16:58:57 by tg               ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lexer.h"
#include "libft.h"
#include <stdbool.h>
#include <stddef.h>

/*
 * - syntax error list
 * : '... or "...(unclosed quotes), ..| |..(check_empty_between_pipes)
 *    , |...(start with pipe)
 *
 * - [UB] Known bug
 * :  ... |(pipe ending line)
 *
 * - NOT error, but treat it as JUST a STRING
 * : ;(semicolon), \(backlash), &(ampersand, background operation)
 *   , *(wildcard), ||(double pipe), &&(double ampersand)
 *  because... subject file page8, line1: "Not interpret unclosed quotes
 *             or special characters which are not required by the subject"
 */
static bool	is_single_pipe(const char *line, size_t index)
{
	size_t	len;

	if (line == NULL)
		return (false);
	len = ft_strlen(line);
	if (line == NULL || line[index] != '|')
		return (false);
	if (index > 0 && line[index - 1] == '|')
		return (false);
	if (len > 0 && line[index + 1] == '|')
		return (false);
	return (true);
}

static bool	check_unclosed_quotes(const char *line)
{
	bool	in_single_quote;
	bool	in_double_quote;

	in_single_quote = false;
	in_double_quote = false;
	while (line != NULL && *line != '\0')
	{
		if (*line == '\'' && !in_double_quote)
			in_single_quote = !in_single_quote;
		else if (*line == '"' && !in_single_quote)
			in_double_quote = !in_double_quote;
		line++;
	}
	return (in_single_quote || in_double_quote);
}

static bool	check_empty_between_pipes(const char *line)
{
	size_t	index;
	size_t	next;
	bool	in_s_quote;
	bool	in_d_quote;

	index = 0;
	in_s_quote = false;
	in_d_quote = false;
	while (line != NULL && line[index] != '\0')
	{
		if (line[index] == '\'' && !in_d_quote)
			in_s_quote = !in_s_quote;
		else if (line[index] == '"' && !in_s_quote)
			in_d_quote = !in_d_quote;
		else if (!in_s_quote && !in_d_quote && is_single_pipe(line, index))
		{
			next = index + 1;
			while (line[next] == ' ')
				next++;
			if (is_single_pipe(line, next))
				return (true);
		}
		index++;
	}
	return (false);
}

static bool	check_start_with_pipe(const char *line)
{
	if (line == NULL)
		return (false);
	while (*line == ' ')
		line++;
	return (is_single_pipe(line, 0));
}

t_status	lexer_check_syntax(const char *line)
{
	if (check_unclosed_quotes(line))
		return (FAIL);
	else if (check_empty_between_pipes(line))
		return (FAIL);
	else if (check_start_with_pipe(line))
		return (FAIL);
	return (OK);
}
