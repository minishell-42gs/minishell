/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_factory.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: taegokim <taegokim@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/29 10:59:32 by taegokim          #+#    #+#             */
/*   Updated: 2026/07/29 16:15:22 by taegokim         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cmd.h"
#include "libft.h"
#include <stddef.h>
#include <stdlib.h>

// argv is always null-terminate
// redirs : hard coding NULL
t_cmd	*cmd_factory_create(t_cmd_factory *this, t_token **token)
{
	t_cmd			*cmd;
	char			*str;

	(void)this;
	cmd = ft_calloc(1, sizeof(t_cmd));
	if (!cmd)
		return (NULL);
	if (cmd_init(cmd) != OK)
		return (free(cmd), NULL);
	while (*token != NULL)
	{
		if ((*token)->type == TOKEN_WORD)
		{
			str = ft_strdup((*token)->value);
			if (!str)
				return (cmd->destroy(cmd), free(cmd), NULL);
			if (cmd_append_argv(cmd, str) != OK)
				return (cmd->destroy(cmd), free(cmd), free(str), NULL);
		}
		else
			break ;
		(*token) = (*token)->next;
	}
	cmd->redirs = NULL;
	return (cmd);
}

static void	destroy_impl(t_cmd_factory *this)
{
	(void)this;
}

t_status	cmd_factory_init(t_cmd_factory *this)
{
	this->destroy = destroy_impl;
	return (OK);
}
