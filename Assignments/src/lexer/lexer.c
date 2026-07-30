/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: taegokim <taegokim@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/26 08:24:21 by taegokim          #+#    #+#             */
/*   Updated: 2026/07/30 19:11:35 by taegokim         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lexer.h"
#include "libft.h"
#include "util.h"

t_status	lexer_run(t_lexer *this, const char *line, t_token_list *token_list)
{
	char	**splited;
	size_t	i;
	t_token	*token;

	if (line == NULL || token_list == NULL)
		return (FAIL);
	splited = ft_split(line, ' ');
	if (!splited)
		return (FAIL);
	i = -1;
	while (splited[++i] != NULL)
	{
		token = token_factory_create(&this->token_factory, splited[i]);
		if (!token)
			return (free_split(splited), FAIL);
		if (token_list_add_token(token_list, token) != OK)
			return (token->destroy(token), free(token), free_split(splited),
				FAIL);
	}
	return (free_split(splited), OK);
}

static void	destroy_impl(t_lexer *this)
{
	if (this->token_factory.destroy != NULL)
		this->token_factory.destroy(&this->token_factory);
}

t_status	lexer_init(t_lexer *this)
{
	this->destroy = destroy_impl;
	if (token_factory_init(&this->token_factory) != OK)
		return (FAIL);
	return (OK);
}
