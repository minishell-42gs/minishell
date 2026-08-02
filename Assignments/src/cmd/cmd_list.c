/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_list.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: taegokim <taegokim@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/29 10:08:51 by taegokim          #+#    #+#             */
/*   Updated: 2026/07/29 16:37:39 by taegokim         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cmd.h"
#include "util.h"
#include <stddef.h>
#include <stdlib.h>

static void	destroy_all_cmds(t_cmd *cmds)
{
	t_cmd	*temp;
	t_cmd	*cmd;

	cmd = cmds;
	while (cmd != NULL)
	{
		temp = cmd->next;
		cmd->destroy(cmd);
		free(cmd);
		cmd = temp;
	}
}

static void	destroy_impl(t_cmd_list *this)
{
	if (this->head != NULL)
		destroy_all_cmds(this->head);
}

t_status	cmd_list_add_cmd(t_cmd_list *this, t_cmd *new_node)
{
	t_cmd	*cmd;

	if (this == NULL || new_node == NULL)
		return (FAIL);
	if (this->head == NULL)
	{
		this->head = new_node;
		return (OK);
	}
	cmd = this->head;
	while (cmd->next != NULL)
		cmd = cmd->next;
	cmd->next = new_node;
	return (OK);
}

t_status	cmd_list_init(t_cmd_list *this)
{
	this->destroy = destroy_impl;
	this->head = NULL;
	return (OK);
}
