/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: taegokim <taegokim@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/28 17:04:15 by taegokim          #+#    #+#             */
/*   Updated: 2026/07/29 09:34:24 by taegokim         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "token.h"
#include <stddef.h>

static void	destroy_impl(t_token *this)
{
	if (this->value != NULL)
		free(this->value);
}

t_status	token_init(t_token *this, t_token_type type, char *value)
{
	this->type = type;
	this->value = value;
	this->next = NULL;
	return (OK);
}
