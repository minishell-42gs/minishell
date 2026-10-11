/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   built_in_pwd.c                                     :+:      :+:    :+:   */
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

int	builtin_pwd(t_cmd *cmd)
{
	char	path[4096];

	if (builtin_arg_count(cmd) > 1)
		return (builtin_error("pwd", "too many arguments"), 1);
	if (getcwd(path, sizeof(path)) == NULL)
		return (builtin_error("pwd", "cannot get current directory"), 1);
	ft_putendl_fd(path, STDOUT_FILENO);
	return (0);
}
