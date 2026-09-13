/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_facade.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: taegokim <taegokim@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/26 11:08:14 by taegokim          #+#    #+#             */
/*   Updated: 2026/07/29 14:15:11 by taegokim         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parsing_facade.h"
#include <stddef.h>

static void	destroy_impl(t_parsing_facade *this)
{
	if (this->lexer.destroy != NULL)
		this->lexer.destroy(&this->lexer);
	if (this->parser.destroy != NULL)
		this->parser.destroy(&this->parser);
}

t_status	parsing_facade_init(t_parsing_facade *this, t_env_list *env_list)
{
	if (this == NULL)
		return (FAIL);
	this->env_list = env_list;
	this->destroy = destroy_impl;
	if (lexer_init(&this->lexer) != OK)
		return (FAIL);
	if (parser_init(&this->parser) != OK)
		return (FAIL);
	return (OK);
}

static t_parse_outcome	internal_error(const char *detail)
{
	t_parse_outcome	outcome;

	outcome.result = PARSE_FATAL_ERROR;
	outcome.has_error_req = true;
	outcome.error = (t_error_req){ERR_INTERNAL, 1, {.s_internal = {detail}}};
	return (outcome);
}

static t_parse_outcome	lexer_error(t_token_list *token_list,
		const char *syntax_token)
{
	t_parse_outcome	outcome;

	token_list->destroy(token_list);
	if (syntax_token == NULL)
		return (internal_error("lexer failed"));
	outcome.result = PARSE_SYNTAX_ERROR;
	outcome.has_error_req = true;
	outcome.error = (t_error_req){ERR_SYNTAX, 2,
	{.s_syntax = {syntax_token}}};
	return (outcome);
}

t_parse_outcome	parsing_facade_parse(t_parsing_facade *this, const char *line,
		t_cmd_list *cmd_list)
{
	t_token_list	token_list;
	const char		*syntax_token;
	t_parse_outcome	outcome;

	outcome = (t_parse_outcome){PARSE_OK, false, {0}};
	if (this == NULL || line == NULL || cmd_list == NULL)
		return (internal_error("invalid parser input"));
	if (token_list_init(&token_list) != OK)
		return (internal_error("token list initialization failed"));
	syntax_token = NULL;
	if (lexer_run(&this->lexer, line, &token_list, &syntax_token) != OK)
		return (lexer_error(&token_list, syntax_token));
	if (parser_run(&this->parser, token_list.head, cmd_list) != OK)
	{
		token_list.destroy(&token_list);
		return (internal_error("parser failed"));
	}
	token_list.destroy(&token_list);
	return (outcome);
}
