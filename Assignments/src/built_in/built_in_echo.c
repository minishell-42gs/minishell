/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   built_in_echo.c                                    :+:      :+:    :+:   */
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

static int	is_n_option(const char *arg)
{
	int	i;

	if (arg == NULL || arg[0] != '-' || arg[1] != 'n')
		return (0);
	i = 1;
	while (arg[i] == 'n')
		i++;
	return (arg[i] == '\0');
}

int	builtin_echo(t_cmd *cmd)
{
	int	i;
	int	newline;
	int	printed;

	i = 1;
	newline = 1;
	printed = 0;
	while (is_n_option(cmd->argv[i]))
	{
		newline = 0;
		i++;
	}
	while (cmd->argv[i] != NULL)
	{
		if (printed && write(1, " ", 1) < 0)
			return (1);
		if (write(1, cmd->argv[i], ft_strlen(cmd->argv[i])) < 0)
			return (1);
		printed = 1;
		i++;
	}
	if (newline && write(1, "\n", 1) < 0)
		return (1);
	return (0);
}
