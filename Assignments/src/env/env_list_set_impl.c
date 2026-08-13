/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_list_set_impl.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tg <tg@student.42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/09 10:48:39 by tg                #+#    #+#             */
/*   Updated: 2026/08/13 17:27:34 by tg               ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "env.h"
#include "libft.h"
#include "util.h"
#include <stddef.h>

static t_status	change_env_value(t_env *env, const char *new_value)
{
	char	*n_val;

	n_val = ft_strdup(new_value);
	if (!n_val)
		return (FAIL);
	free(env->value);
	env->value = n_val;
	return (OK);
}

/*
 * brief: set a env. if key is already exist, it will be EXCHANGED. NOT EXPAND.
 * usage: env_list_set_impl(list, "PATH", "/usr/bin:/usr/bin:~/my_path")
 * return: OK
 */
t_status	env_list_set(t_env_list *this, const char *key,
		const char *value)
{
	t_env	*env;
	t_env	*new_env;
	t_env	*prev;

	if (key == NULL || value == NULL)
		return (FAIL);
	env = this->head;
	while (env != NULL)
	{
		if (is_same_str(key, env->key))
			return (change_env_value(env, value));
		prev = env;
		env = env->next;
	}
	new_env = create_env(key, value);
	if (!new_env)
		return (FAIL);
	if (this->head == NULL)
		this->head = new_env;
	else
		prev->next = new_env;
	return (OK);
}
