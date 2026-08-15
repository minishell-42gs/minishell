/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tg <tg@student.42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/08 14:59:25 by tg                #+#    #+#             */
/*   Updated: 2026/08/15 09:10:26 by tg               ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "env.h"
#include "libft.h"
#include <stddef.h>
#include <stdlib.h>

static void	destroy_impl(t_env *this)
{
	free(this->key);
	free(this->value);
}

t_env	*create_env(const char *key, const char *value)
{
	t_env	*new_node;

	if (key == NULL || value == NULL)
		return (NULL);
	new_node = ft_calloc(1, sizeof(t_env));
	if (!new_node)
		return (NULL);
	if (env_init(new_node, key, value) != OK)
		return (free(new_node), NULL);
	return (new_node);
}

t_status	env_init(t_env *this, const char *key, const char *value)
{
	this->destroy = destroy_impl;
	this->key = ft_strdup(key);
	if (this->key == NULL)
		return (FAIL);
	this->value = ft_strdup(value);
	if (this->value == NULL)
		return (free(this->key), FAIL);
	this->next = NULL;
	return (OK);
}
