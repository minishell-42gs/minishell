/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_list.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: taegokim <taegokim@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/28 13:18:18 by taegokim          #+#    #+#             */
/*   Updated: 2026/07/29 09:25:56 by taegokim         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "token.h"
#include <stddef.h>

static void	destroy_all_tokens(t_token *tokens)
{
	t_token	*temp;
	t_token	*node;

	node = tokens;
	while (node != NULL)
	{
		temp = node->next;
		node->destroy(node);
		free(node);
		node = temp;
	}
}

static void	destroy_impl(t_token_list *this)
{
	if (this->head != NULL)
		destroy_all_tokens(this->head);
	if (this->token_factory.destroy != NULL)
		this->token_factory.destroy(&this->token_factory);
}

static void	add_token_impl(t_token_list *this, t_token *new_node)
{
	t_token	*node;

	if (this->head == NULL)
	{
		this->head = new_node;
		return ;
	}
	node = this->head;
	while (node->next != NULL)
		node = node->next;
	(*node).next = new_node;
}

t_status	token_list_init(t_token_list *this)
{
	this->destroy = destroy_impl;
	this->add_token = add_token_impl;
	this->head = NULL;
	if (token_factory_init(&this->token_factory) != OK)
		return (FAIL);
	return (OK);
}
