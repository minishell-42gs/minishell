/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tg <tg@student.42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/26 08:24:21 by taegokim          #+#    #+#             */
/*   Updated: 2026/08/30 17:23:53 by tg               ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lexer.h"
#include <stddef.h>

t_status	lexer_run(t_lexer *this, const char *line, t_token_list *token_list)
{
	if (this == NULL || line == NULL || token_list == NULL)
		return (FAIL);
	if (lexer_check_syntax(line) != OK)
		return (FAIL);
	if (lexer_tokenize(line, token_list) != OK)
		return (FAIL);
	return (OK);
}

static void	destroy_impl(t_lexer *this)
{
	(void)this;
}

t_status	lexer_init(t_lexer *this)
{
	if (this == NULL)
		return (FAIL);
	this->destroy = destroy_impl;
	return (OK);
}
