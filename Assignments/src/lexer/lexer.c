/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: taegokim <taegokim@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/26 08:24:21 by taegokim          #+#    #+#             */
/*   Updated: 2026/07/28 17:57:23 by taegokim         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lexer.h"
#include "libft.h"
#include "util.h"

static t_status	run_impl(t_lexer *this, const char *line, t_tokens *tokens)
{
	char			**splited;
	size_t			i;
	t_token_node	*token_node;

	// 빈 라인 등, 예외 처리 필요할 듯?
	(void)this;
	splited = ft_split(line, ' ');
	if (!splited)
		return (FAIL);
	i = -1;
	while (splited[++i] != NULL)
	{
		token_node = tokens->token_factory.create(&tokens->token_factory,
				splited[i]);
		if (!token_node)
			return (free_split(splited), FAIL);
		tokens->add_token(tokens, token_node);
	}
	return (free_split(splited), OK);
}

static void	destroy_impl(t_lexer *this)
{
	(void)this;
}

t_status	lexer_init(t_lexer *this)
{
	this->run = run_impl;
	this->destroy = destroy_impl;
	return (OK);
}
