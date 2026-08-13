/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_list_get_impl.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tg <tg@student.42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/09 10:32:38 by tg                #+#    #+#             */
/*   Updated: 2026/08/13 16:18:49 by tg               ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "env.h"
#include "libft.h"
#include "util.h"
#include <stddef.h>

/*
 * brief: get a env value from key
 * usage: env_list_get_impl(list, "PATH")
 * return: /usr/local/sbin:/usr/local/bin:/usr/sbin
 */
char	*env_list_get_impl(t_env_list *this, const char *key)
{
	t_env	*env;

	if (key == NULL)
		return (NULL);
	env = this->head;
	while (env != NULL)
	{
		if (is_same_str(key, env->key))
			return (env->value);
		env = env->next;
	}
	return (NULL);
}
