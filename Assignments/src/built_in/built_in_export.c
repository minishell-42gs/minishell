/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   built_in_export.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tg <tg@student.42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/04 15:00:00 by tg                #+#    #+#             */
/*   Updated: 2026/10/04 15:00:00 by tg               ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "built_in_util.h"
#include "libft.h"
#include <stdlib.h>

static int	valid_assignment(const char *arg, size_t *equal)
{
	size_t	i;

	i = 0;
	while (arg[i] != '\0' && arg[i] != '=')
		i++;
	*equal = i;
	return (i > 0 && builtin_valid_name(arg));
}

static int	set_assignment(t_env_list *env, const char *arg)
{
	char	*key;
	char	*value;
	size_t	equal;
	int		status;

	if (!valid_assignment(arg, &equal))
		return (builtin_error("export", "not a valid identifier"), 1);
	if (arg[equal] == '\0')
	{
		if (env_list_declare(env, arg) != OK)
			return (1);
		return (0);
	}
	key = ft_substr(arg, 0, equal);
	value = ft_strdup(arg + equal + 1);
	if (key == NULL || value == NULL)
		return (free(key), free(value), 1);
	status = env_list_set(env, key, value) != OK;
	free(key);
	free(value);
	return (status);
}

static void	print_item(t_env *item)
{
	ft_putstr_fd("declare -x ", 1);
	ft_putstr_fd(item->key, 1);
	if (item->has_value)
	{
		ft_putstr_fd("=\"", 1);
		ft_putstr_fd(item->value, 1);
		ft_putstr_fd("\"", 1);
	}
	ft_putstr_fd("\n", 1);
}

static void	print_export(t_env_list *env)
{
	t_env	*item;
	t_env	*scan;
	char	*previous;

	previous = NULL;
	while (1)
	{
		item = NULL;
		scan = env->head;
		while (scan != NULL)
		{
			if ((previous == NULL || ft_strncmp(scan->key, previous,
						ft_strlen(previous) + 1) > 0)
				&& (item == NULL || ft_strncmp(scan->key, item->key,
						ft_strlen(item->key) + 1) < 0))
				item = scan;
			scan = scan->next;
		}
		if (item == NULL)
			return ;
		print_item(item);
		previous = item->key;
	}
}

int	builtin_export(t_cmd *cmd, t_env_list *env)
{
	int	i;
	int	status;

	if (cmd->argv[1] == NULL)
		return (print_export(env), 0);
	i = 1;
	status = 0;
	while (cmd->argv[i] != NULL)
	{
		status |= set_assignment(env, cmd->argv[i]);
		i++;
	}
	return (status);
}
