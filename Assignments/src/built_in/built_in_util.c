/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   built_in_util.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tg <tg@student.42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/04 15:00:00 by tg                #+#    #+#             */
/*   Updated: 2026/10/04 15:00:00 by tg               ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "built_in_util.h"
#include "error.h"
#include "libft.h"
#include <stdbool.h>

int	builtin_arg_count(t_cmd *cmd)
{
	int	count;

	count = 0;
	while (cmd->argv[count] != NULL)
		count++;
	return (count);
}

int	builtin_valid_name(const char *name)
{
	int	i;

	if (name == NULL || !(ft_isalpha(*name) || *name == '_'))
		return (0);
	i = 1;
	while (name[i] != '\0' && name[i] != '=')
	{
		if (!(ft_isalnum(name[i]) || name[i] == '_'))
			return (0);
		i++;
	}
	return (i > 0);
}

int	builtin_error(const char *name, const char *detail)
{
	t_error_req	req;

	req = (t_error_req){ERR_BUILTIN, 1,
	{.s_builtin = {name, detail}}};
	error_report(NULL, &req);
	return (1);
}
