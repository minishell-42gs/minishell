/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_list_extra.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tg <tg@student.42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/04 15:00:00 by tg                #+#    #+#             */
/*   Updated: 2026/10/04 15:00:00 by tg               ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "env.h"
#include "libft.h"
#include <stdlib.h>

static t_env	*find_env(t_env_list *list, const char *key)
{
	t_env	*env;

	env = list->head;
	while (env != NULL)
	{
		if (ft_strncmp(env->key, key, ft_strlen(key) + 1) == 0)
			return (env);
		env = env->next;
	}
	return (NULL);
}

t_status	env_list_declare(t_env_list *this, const char *key)
{
	t_env	*env;
	t_env	*tail;

	if (this == NULL || key == NULL || *key == '\0')
		return (FAIL);
	env = find_env(this, key);
	if (env != NULL)
		return (env->is_exported = true, OK);
	env = create_env(key, "");
	if (env == NULL)
		return (FAIL);
	env->is_exported = true;
	env->has_value = false;
	if (this->head == NULL)
		this->head = env;
	else
	{
		tail = this->head;
		while (tail->next != NULL)
			tail = tail->next;
		tail->next = env;
	}
	return (OK);
}

static t_status	clone_env(t_env_list *dst, t_env *src)
{
	t_env	*copy;
	t_env	*tail;

	copy = create_env(src->key, src->value);
	if (copy == NULL)
		return (FAIL);
	copy->is_exported = src->is_exported;
	copy->has_value = src->has_value;
	if (dst->head == NULL)
		dst->head = copy;
	else
	{
		tail = dst->head;
		while (tail->next != NULL)
			tail = tail->next;
		tail->next = copy;
	}
	return (OK);
}

t_status	env_list_clone(t_env_list *dst, t_env_list *src)
{
	t_env	*env;

	if (dst == NULL || src == NULL)
		return (FAIL);
	if (env_list_init(dst, (char *[]){NULL}) != OK)
		return (FAIL);
	env = src->head;
	while (env != NULL)
	{
		if (clone_env(dst, env) != OK)
			return (dst->destroy(dst), FAIL);
		env = env->next;
	}
	return (OK);
}
