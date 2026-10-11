/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_list.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tg <tg@student.42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/28 13:18:18 by taegokim          #+#    #+#             */
/*   Updated: 2026/08/23 18:30:30 by tg               ###   ########.fr       */
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
	if (this == NULL)
		return ;
	if (this->head != NULL)
		destroy_all_tokens(this->head);
	this->head = NULL;
	this->tail = NULL;
}

t_status	token_list_add_token(t_token_list *this, t_token *new_node)
{
	if (this == NULL || new_node == NULL)
		return (FAIL);
	if (this->head == NULL)
	{
		this->head = new_node;
		this->tail = new_node;
		while (this->tail->next != NULL)
			this->tail = this->tail->next;
		return (OK);
	}
	this->tail->next = new_node;
	while (this->tail->next != NULL)
		this->tail = this->tail->next;
	return (OK);
}

t_status	token_list_init(t_token_list *this)
{
	if (this == NULL)
		return (FAIL);
	this->destroy = destroy_impl;
	this->head = NULL;
	this->tail = NULL;
	return (OK);
}
