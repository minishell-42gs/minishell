/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_list_to_envp_impl.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tg <tg@student.42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/09 10:48:44 by tg                #+#    #+#             */
/*   Updated: 2026/08/13 16:16:58 by tg               ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "env.h"
#include "libft.h"
#include "util.h"
#include <stddef.h>

static char	*env_to_str(t_env *env)
{
	char	*env_str;
	char	*key_equal;

	key_equal = ft_strjoin(env->key, "=");
	if (!key_equal)
		return (NULL);
	env_str = ft_strjoin(key_equal, env->value);
	if (!env_str)
		return (free(key_equal), NULL);
	free(key_equal);
	return (env_str);
}

static void	free_envp(char **envp)
{
	free_split(envp);
}

static t_status	append_envp(char ***envp, char *env_str)
{
	int		i;
	int		size;
	char	**new_envp;

	size = 0;
	i = -1;
	while ((*envp)[++i])
		size++;
	new_envp = ft_calloc(size + 2, sizeof(char *));
	if (!new_envp)
		return (FAIL);
	i = -1;
	while (++i < size)
		new_envp[i] = (*envp)[i];
	new_envp[i] = env_str;
	free(*envp);
	*envp = new_envp;
	return (OK);
}

/*
 * brief: make 'envp' parameter for `execve(path, argv, envp)`
 * usage: env_list_unset_impl(env_list);
 * return: {"PATH=/usr/bin:/usr/sbin:/usr/local/bin", "NAME=taegon",..., NULL}
 */
char	**env_list_to_envp_impl(t_env_list *this)
{
	t_env	*env;
	char	*env_str;
	char	**envp;

	env = this->head;
	envp = ft_calloc(1, sizeof(char *));
	while (env != NULL)
	{
		env_str = env_to_str(env);
		if (env_str == NULL)
			return (free_envp(envp), NULL);
		if (append_envp(&envp, env_str) != OK)
			return (free_envp(envp), free(env_str), NULL);
		env = env->next;
	}
	return (envp);
}
