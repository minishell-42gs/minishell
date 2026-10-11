/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   built_in_cd.c                                      :+:      :+:    :+:   */
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
#include <unistd.h>

static char	*cd_target(t_cmd *cmd, t_env_list *env)
{
	char	*target;

	if (cmd->argv[1] == NULL)
	{
		target = env->get(env, "HOME");
		if (target == NULL)
			builtin_error("cd", "HOME not set");
		return (target);
	}
	if (ft_strncmp(cmd->argv[1], "-", 2) == 0)
	{
		target = env->get(env, "OLDPWD");
		if (target == NULL)
			builtin_error("cd", "OLDPWD not set");
		return (target);
	}
	return (cmd->argv[1]);
}

static int	change_directory(char *target, char *old_path, char *new_path)
{
	if (getcwd(old_path, 4096) == NULL)
		old_path[0] = '\0';
	if (chdir(target) == -1 || getcwd(new_path, 4096) == NULL)
		return (builtin_error("cd", "No such file or directory"), 1);
	return (0);
}

static int	update_pwd(t_env_list *env, char *old_path, char *new_path)
{
	if (env_list_set(env, "OLDPWD", old_path) != OK)
		return (1);
	if (env_list_set(env, "PWD", new_path) != OK)
		return (1);
	return (0);
}

int	builtin_cd(t_cmd *cmd, t_env_list *env)
{
	char	old_path[4096];
	char	new_path[4096];
	char	*target;

	if (builtin_arg_count(cmd) > 2)
		return (builtin_error("cd", "too many arguments"), 1);
	target = cd_target(cmd, env);
	if (target == NULL)
		return (1);
	if (change_directory(target, old_path, new_path) != 0)
		return (1);
	if (update_pwd(env, old_path, new_path) != 0)
		return (1);
	if (cmd->argv[1] != NULL && ft_strncmp(cmd->argv[1], "-", 2) == 0)
		ft_putendl_fd(new_path, STDOUT_FILENO);
	return (0);
}
