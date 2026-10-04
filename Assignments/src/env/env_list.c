/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_list.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tg <tg@student.42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/08 14:58:46 by tg                #+#    #+#             */
/*   Updated: 2026/08/15 09:23:59 by tg               ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "env.h"
#include "libft.h"
#include <stddef.h>
#include <stdlib.h>

static void	destroy_all_envs(t_env *head)
{
	t_env	*temp;
	t_env	*env;

	env = head;
	while (env != NULL)
	{
		temp = env->next;
		env->destroy(env);
		free(env);
		env = temp;
	}
}

static void	destroy_impl(t_env_list *this)
{
	if (this->head != NULL)
	{
		destroy_all_envs(this->head);
		this->head = NULL;
	}
}

static t_status	env_list_init_from_envp(t_env_list *list, char **envp)
{
	size_t	i;
	char	*equal;
	char	*value;
	char	*key;

	if (envp == NULL)
		return (FAIL);
	i = -1;
	while (envp[++i] != NULL)
	{
		equal = ft_strchr(envp[i], '=');
		if (equal == NULL || equal == envp[i])
			continue ;
		key = ft_substr(envp[i], 0, equal - envp[i]);
		if (!key)
			return (FAIL);
		value = equal + 1;
		if (env_list_set(list, key, value) != OK)
			return (free(key), FAIL);
		free(key);
	}
	return (OK);
}

t_status	env_list_init(t_env_list *this, char **envp)
{
	this->destroy = destroy_impl;
	this->get = env_list_get_impl;
	this->to_envp = env_list_to_envp_impl;
	this->head = NULL;
	if (env_list_init_from_envp(this, envp) != OK)
		return (FAIL);
	return (OK);
}
