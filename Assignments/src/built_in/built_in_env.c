/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   built_in_env.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tg <tg@student.42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/04 15:00:00 by tg                #+#    #+#             */
/*   Updated: 2026/10/04 15:00:00 by tg               ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "built_in_util.h"
#include "libft.h"
#include <unistd.h>

int	builtin_env(t_cmd *cmd, t_env_list *env)
{
	t_env	*item;

	if (builtin_arg_count(cmd) != 1)
		return (builtin_error("env", "too many arguments"), 1);
	item = env->head;
	while (item != NULL)
	{
		if (item->is_exported && item->has_value)
		{
			ft_putstr_fd(item->key, STDOUT_FILENO);
			write(STDOUT_FILENO, "=", 1);
			ft_putendl_fd(item->value, STDOUT_FILENO);
		}
		item = item->next;
	}
	return (0);
}
