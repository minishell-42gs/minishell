/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_factory.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: taegokim <taegokim@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/28 15:25:34 by taegokim          #+#    #+#             */
/*   Updated: 2026/07/29 16:21:26 by taegokim         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "token.h"
#include "libft.h"
#include <stddef.h>
#include <stdlib.h>

static t_token_type	classify_token_type(const char *str)
{
	if (ft_strlen(str) == 1 && ft_strncmp(str, "|", 1) == 0)
		return (TOKEN_PIPE);
	return (TOKEN_WORD);
}

static t_token	*create_impl(t_token_factory *this, const char *_value)
{
	t_token			*token;
	char			*value;
	t_token_type	type;

	(void)this;
	token = ft_calloc(1, sizeof(t_token));
	if (!token)
		return (NULL);
	value = ft_strdup(_value);
	if (!value)
		return (free(token), NULL);
	type = classify_token_type(value);
	if (token_init(token, type, value) != OK)
		return (free(token), free(value), NULL);
	return (token);
}

static void	destroy_impl(t_token_factory *this)
{
	(void)this;
}

t_status	token_factory_init(t_token_factory *this)
{
	this->create = create_impl;
	this->destroy = destroy_impl;
	return (OK);
}
