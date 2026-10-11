/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   built_in_exit.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tg <tg@student.42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/04 15:00:00 by tg                #+#    #+#             */
/*   Updated: 2026/10/04 15:00:00 by tg               ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "built_in_util.h"
#include "libft.h"
#include <limits.h>
#include <stdbool.h>

static int	parse_exit_number(const char *arg, int *result)
{
	unsigned long	value;
	unsigned long	limit;
	int				negative;
	size_t			i;

	i = 0;
	negative = 0;
	if (arg[i] == '+' || arg[i] == '-')
		negative = arg[i++] == '-';
	if (arg[i] == '\0')
		return (0);
	value = 0;
	limit = ULONG_MAX >> 1;
	while (arg[i] != '\0')
	{
		if (!ft_isdigit(arg[i]) || value > (limit - (arg[i] - '0')) / 10)
			return (0);
		value = value * 10 + arg[i++] - '0';
	}
	if (negative)
		value = 0 - value;
	*result = (unsigned char)value;
	return (1);
}

int	builtin_exit(t_cmd *cmd, int *status, bool *exit_requested)
{
	int	value;

	*exit_requested = false;
	if (cmd->argv[1] == NULL)
		return (*exit_requested = true, *status);
	if (!parse_exit_number(cmd->argv[1], &value))
	{
		builtin_error("exit", "numeric argument required");
		*exit_requested = true;
		return (2);
	}
	if (cmd->argv[2] != NULL)
		return (builtin_error("exit", "too many arguments"), 1);
	*exit_requested = true;
	return (value);
}
