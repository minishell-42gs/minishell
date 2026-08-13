/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_list_unset_impl.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tg <tg@student.42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/09 10:48:50 by tg                #+#    #+#             */
/*   Updated: 2026/08/13 16:54:14 by tg               ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "env.h"
#include "util.h"
#include <stddef.h>
#include <stdlib.h>

/*
 * brief: delete env. if that key does not exsit, func returns OK.
 * usage: env_list_unset_impl(list, "MY_ENV");
 * return: OK
 */
t_status	env_list_unset_impl(t_env_list *this, const char *key)
{
	t_env	*env;
	t_env	*prev;

	if (key == NULL)
		return (FAIL);
	env = this->head;
	prev = NULL;
	while (env != NULL)
	{
		if (is_same_str(key, env->key))
		{
			if (prev == NULL)
				this->head = env->next;
			else
				prev->next = env->next;
			env->destroy(env);
			free(env);
			return (OK);
		}
		prev = env;
		env = env->next;
	}
	return (OK);
}
