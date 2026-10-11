/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   built_in.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tg <tg@student.42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/13 17:30:00 by tg                #+#    #+#             */
/*   Updated: 2026/10/04 15:00:00 by tg               ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "built_in.h"
#include "built_in_util.h"
#include "libft.h"
#include <stddef.h>

static bool	is_built_in_impl(t_built_in *this, const char *name)
{
	(void)this;
	return (name != NULL && (ft_strncmp(name, "echo", 5) == 0
			|| ft_strncmp(name, "cd", 3) == 0
			|| ft_strncmp(name, "pwd", 4) == 0
			|| ft_strncmp(name, "export", 7) == 0
			|| ft_strncmp(name, "unset", 6) == 0
			|| ft_strncmp(name, "env", 4) == 0
			|| ft_strncmp(name, "exit", 5) == 0));
}

static int	dispatch(t_cmd *cmd, t_env_list *env, t_builtin_result *result)
{
	char	*name;

	name = cmd->argv[0];
	if (ft_strncmp(name, "echo", 5) == 0)
		return (builtin_echo(cmd));
	if (ft_strncmp(name, "cd", 3) == 0)
		return (builtin_cd(cmd, env));
	if (ft_strncmp(name, "pwd", 4) == 0)
		return (builtin_pwd(cmd));
	if (ft_strncmp(name, "export", 7) == 0)
		return (builtin_export(cmd, env));
	if (ft_strncmp(name, "unset", 6) == 0)
		return (builtin_unset(cmd, env));
	if (ft_strncmp(name, "env", 4) == 0)
		return (builtin_env(cmd, env));
	return (builtin_exit(cmd, &result->status, &result->exit_requested));
}

static t_status	run_impl(t_built_in *this, t_cmd *cmd,
		t_env_list *env, t_builtin_result *result)
{
	(void)this;
	if (cmd == NULL || cmd->argv[0] == NULL || result == NULL)
		return (FAIL);
	result->status = dispatch(cmd, env, result);
	return (OK);
}

static void	destroy_impl(t_built_in *this)
{
	(void)this;
}

t_status	built_in_init(t_built_in *this)
{
	if (this == NULL)
		return (FAIL);
	this->is_built_in = is_built_in_impl;
	this->run = run_impl;
	this->destroy = destroy_impl;
	return (OK);
}
