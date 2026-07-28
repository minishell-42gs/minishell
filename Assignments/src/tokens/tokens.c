/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokens.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: taegokim <taegokim@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/28 13:18:18 by taegokim          #+#    #+#             */
/*   Updated: 2026/07/28 18:10:58 by taegokim         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "tokens.h"
#include <stddef.h>

static void	destroy_all_tokens(t_token_node *tokens)
{
	t_token_node	*temp;
	t_token_node	*node;

	node = tokens;
	while (node != NULL)
	{
		temp = node->next;
		node->destroy(node);
		free(node);
		node = temp;
	}
}

static void	destroy_impl(t_tokens *this)
{
	if (this->tokens != NULL)
		destroy_all_tokens(this->tokens);
	if (this->token_factory.destroy != NULL)
		this->token_factory.destroy(&this->token_factory);
}

static void	add_token_impl(t_tokens *this, t_token_node *new_node)
{
	t_token_node	*node;

	if (this->tokens == NULL)
	{
		this->tokens = new_node;
		return ;
	}
	node = this->tokens;
	while (node->next != NULL)
		node = node->next;
	(*node).next = new_node;
}

t_status	tokens_init(t_tokens *this)
{
	this->destroy = destroy_impl;
	this->add_token = add_token_impl;
	this->tokens = NULL;
	if (token_factory_init(&this->token_factory) != OK)
		return (FAIL);
	return (OK);
}
