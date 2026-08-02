/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_list.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: taegokim <taegokim@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/28 13:18:18 by taegokim          #+#    #+#             */
/*   Updated: 2026/07/29 16:01:32 by taegokim         ###   ########.fr       */
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
}

t_status	token_list_add_token(t_token_list *this, t_token *new_node)
{
	t_token	*node;

	if (this == NULL || new_node == NULL)
		return (FAIL);
	if (this->head == NULL)
	{
		this->head = new_node;
		return (OK);
	}
	node = this->head;
	while (node->next != NULL)
		node = node->next;
	(*node).next = new_node;
	return (OK);
}

t_status	token_list_init(t_token_list *this)
{
	this->destroy = destroy_impl;
	this->head = NULL;
	return (OK);
}
