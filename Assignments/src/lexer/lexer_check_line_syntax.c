/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_check_line_syntax.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tg <tg@student.42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/04 15:00:00 by tg                #+#    #+#             */
/*   Updated: 2026/10/04 15:00:00 by tg               ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lexer.h"
#include "libft.h"
#include "util.h"
#include <stdbool.h>
#include <stddef.h>

/*
 * - syntax error list
 * : '... or "...(unclosed quotes), ..| |..(check_pipe_syntax)
 *    , ... |(pipe ending line), |...(start with pipe)
 *
 * - NOT error, but treat it as JUST a STRING
 * : ;(semicolon), \(backlash), &(ampersand, background operation)
 *   , *(wildcard), ||(double pipe), &&(double ampersand)
 *  because... subject file page8, line1: "Not interpret unclosed quotes
 *             or special characters which are not required by the subject"
 */
static bool	is_single_pipe(const char *line, size_t index)
{
	if (line == NULL || line[index] != '|')
		return (false);
	if (index > 0 && line[index - 1] == '|')
		return (false);
	if (line[index + 1] == '|')
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

static bool	check_pipe_syntax(const char *line)
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
			while (is_space(line[next]))
				next++;
			if (is_single_pipe(line, next))
				return (true);
		}
		index++;
	}
	return (false);
}

static const char	*check_boundary_pipe(const char *line)
{
	size_t	end;

	if (line == NULL)
		return (NULL);
	while (is_space(*line))
		line++;
	if (is_single_pipe(line, 0))
		return ("|");
	end = ft_strlen(line);
	while (end > 0 && is_space(line[end - 1]))
		end--;
	if (end > 0 && is_single_pipe(line, end - 1))
		return ("newline");
	return (NULL);
}

t_status	lexer_check_syntax(const char *line, const char **syntax_token)
{
	const char	*token;

	token = check_boundary_pipe(line);
	if (check_unclosed_quotes(line))
		token = "newline";
	else if (token == NULL && check_pipe_syntax(line))
		token = "|";
	if (syntax_token != NULL)
		*syntax_token = token;
	if (token != NULL)
		return (FAIL);
	return (OK);
}
