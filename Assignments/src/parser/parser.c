/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: taegokim <taegokim@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/26 08:24:32 by taegokim          #+#    #+#             */
/*   Updated: 2026/07/28 18:23:26 by taegokim         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"

static t_status	run_impl(t_parser *this, const t_tokens *tokens, t_cmd **cmds)
{
}

static void	destroy_impl(t_parser *this)
{
	(void)this;
}

t_status	parser_init(t_parser *this)
{
	this->run = run_impl;
	this->destroy = destroy_impl;
}
