/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_check_syntax.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tg <tg@student.42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/30 15:06:06 by tg                #+#    #+#             */
/*   Updated: 2026/09/13 11:47:27 by tg               ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lexer.h"
#include "libft.h"
#include "util.h"
#include <stdbool.h>
#include <stddef.h>

static t_status	set_syntax(const char **syntax_token, const char *token)
{
	if (syntax_token != NULL)
		*syntax_token = token;
	return (FAIL);
}

static t_status	process_token(t_validation_state *state, t_token *token,
					const char **syntax_token)
{
	if (token->type == TOKEN_WORD)
	{
		state->segment = true;
		state->expect_target = false;
		return (OK);
	}
	if (token->type == TOKEN_PIPE)
	{
		if (!state->segment || state->expect_target)
			return (set_syntax(syntax_token, "|"));
		state->segment = false;
		state->pipe_seen = true;
		return (OK);
	}
	if (state->expect_target)
		return (set_syntax(syntax_token, token->value));
	state->expect_target = true;
	return (OK);
}

t_status	lexer_validate_tokens(t_token *tokens, const char **syntax_token)
{
	t_validation_state	state;

	state = (t_validation_state){false, false, false};
	while (tokens != NULL)
	{
		if (process_token(&state, tokens, syntax_token) != OK)
			return (FAIL);
		tokens = tokens->next;
	}
	if (state.expect_target || (state.pipe_seen && !state.segment))
		return (set_syntax(syntax_token, "newline"));
	return (OK);
}
