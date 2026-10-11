/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_list.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: taegokim <taegokim@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/29 10:08:51 by taegokim          #+#    #+#             */
/*   Updated: 2026/08/09 09:41:44 by taegokim         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cmd.h"
#include "util.h"
#include <stddef.h>
#include <stdlib.h>
#include <unistd.h>

static void	destroy_all_cmds(t_cmd *head)
{
	t_cmd	*temp;
	t_cmd	*cmd;

	cmd = head;
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

void	cmd_list_close_heredocs(t_cmd_list *this)
{
	t_cmd	*cmd;
	t_redir	*redir;

	cmd = this->head;
	while (cmd != NULL)
	{
		redir = cmd->redirs;
		while (redir != NULL)
		{
			if (redir->hd_fd >= 0)
				close(redir->hd_fd);
			redir->hd_fd = -1;
			redir = redir->next;
		}
		cmd = cmd->next;
	}
}

t_status	cmd_list_init(t_cmd_list *this)
{
	this->destroy = destroy_impl;
	this->head = NULL;
	return (OK);
}
