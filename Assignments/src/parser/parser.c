/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tg <tg@student.42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/04 15:00:00 by tg                #+#    #+#             */
/*   Updated: 2026/10/04 15:00:00 by tg               ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"
#include <stddef.h>

t_status	parser_run(t_parser *this, t_token *tokens, t_cmd_list *cmd_list)
{
	(void)this;
	if (tokens == NULL || cmd_list == NULL)
		return (FAIL);
	return (parser_build(tokens, cmd_list));
}

static void	destroy_impl(t_parser *this)
{
	if (this->cmd_factory.destroy != NULL)
		this->cmd_factory.destroy(&this->cmd_factory);
}

t_status	parser_init(t_parser *this)
{
	if (this == NULL)
		return (FAIL);
	this->destroy = destroy_impl;
	return (cmd_factory_init(&this->cmd_factory));
}
