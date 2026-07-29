/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_facade.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: taegokim <taegokim@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/26 11:08:14 by taegokim          #+#    #+#             */
/*   Updated: 2026/07/29 09:46:23 by taegokim         ###   ########.fr       */
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

t_status	parsing_facade_init(t_parsing_facade *this)
{
	this->destroy = destroy_impl;
	if (lexer_init(&this->lexer) != OK)
		return (FAIL);
	if (parser_init(&this->parser) != OK)
		return (FAIL);
	return (OK);
}

t_status	parsing_facade_parse(t_parsing_facade *this, const char *line,
		t_cmd **cmds, char **envp)
{
	t_token_list	tokens;

	(void)envp;
	if (token_list_init(&tokens) != OK)
		return (FAIL);
	if (this->lexer.run(&this->lexer, line, &tokens) != OK)
		return (tokens.destroy(&tokens), FAIL);
	if (this->parser.run(&this->parser, &tokens, cmds) != OK)
		return (tokens.destroy(&tokens), FAIL);
	return (tokens.destroy(&tokens), OK);
}
