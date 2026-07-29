/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: taegokim <taegokim@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/26 08:24:32 by taegokim          #+#    #+#             */
/*   Updated: 2026/07/29 16:31:10 by taegokim         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cmd.h"
#include "parser.h"
#include "token.h"
#include <stddef.h>
#include <stdlib.h>

static t_status	run_impl(t_parser *this, t_token *tokens_head,
		t_cmd_list *cmd_list)
{
	t_token	*token;
	t_cmd	*cmd;

	if (tokens_head == NULL || cmd_list == NULL)
		return (FAIL);
	token = tokens_head;
	while (token != NULL)
	{
		if (token->type == TOKEN_WORD)
		{
			cmd = this->cmd_factory.create(&this->cmd_factory, &token);
			if (!cmd)
				return (FAIL);
			if (cmd_list->add_cmd(cmd_list, cmd) != OK)
				return (cmd->destroy(cmd), free(cmd), FAIL);
		}
		else if (token->type == TOKEN_PIPE)
			token = token->next;
		else
			return (FAIL);
	}
	return (OK);
}

static void	destroy_impl(t_parser *this)
{
	if (this->cmd_factory.destroy != NULL)
		this->cmd_factory.destroy(&this->cmd_factory);
}

t_status	parser_init(t_parser *this)
{
	this->run = run_impl;
	this->destroy = destroy_impl;
	if (cmd_factory_init(&this->cmd_factory) != OK)
		return (FAIL);
	return (OK);
}
