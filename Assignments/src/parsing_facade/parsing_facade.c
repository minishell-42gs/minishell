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
		t_cmd_list *cmd_list, char **envp)
{
	t_token_list	token_list;

	(void)envp;
	if (token_list_init(&token_list) != OK)
		return (FAIL);
	if (lexer_run(&this->lexer, line, &token_list) != OK)
		return (token_list.destroy(&token_list), FAIL);
	if (parser_run(&this->parser, token_list.head, cmd_list) != OK)
		return (token_list.destroy(&token_list), FAIL);
	token_list.destroy(&token_list);
	return (OK);
}
