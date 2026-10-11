/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   built_in_unset.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tg <tg@student.42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/04 15:00:00 by tg                #+#    #+#             */
/*   Updated: 2026/10/04 15:00:00 by tg               ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "built_in_util.h"
#include <stddef.h>

static int	unset_one(t_env_list *env, const char *name)
{
	if (!builtin_valid_name(name) || name[0] == '-')
		return (builtin_error("unset", "not a valid identifier"), 1);
	return (env_list_unset(env, name) != OK);
}

int	builtin_unset(t_cmd *cmd, t_env_list *env)
{
	int	i;
	int	status;

	i = 1;
	status = 0;
	while (cmd->argv[i] != NULL)
	{
		if (unset_one(env, cmd->argv[i]) != 0)
			status = 1;
		i++;
	}
	return (status);
}
