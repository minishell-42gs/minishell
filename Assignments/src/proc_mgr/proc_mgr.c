/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   proc_mgr.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tg <tg@student.42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/13 15:30:00 by tg                #+#    #+#             */
/*   Updated: 2026/09/13 17:30:00 by tg               ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "proc_mgr.h"
#include <stddef.h>
#include <stdlib.h>
#include <unistd.h>

static t_status	run_impl(t_proc_mgr *this, int *out_exit_status);

static void	destroy_impl(t_proc_mgr *this)
{
	if (this == NULL)
		return ;
	this->env_list.destroy(&this->env_list);
	this->io_mgr.destroy(&this->io_mgr);
	free(this->pids);
	this->pids = NULL;
}

static t_status	init_members(t_proc_mgr *this, t_cmd_list *cmd_list,
		t_env_list *env_source, t_built_in *built_in)
{
	t_cmd	*cmd;

	this->run = run_impl;
	this->destroy = destroy_impl;
	this->cmd_list = cmd_list;
	this->pids = NULL;
	this->built_in = built_in;
	this->cmd_count = 0;
	cmd = cmd_list->head;
	while (cmd != NULL)
	{
		this->cmd_count++;
		cmd = cmd->next;
	}
	if (io_mgr_init(&this->io_mgr, this->cmd_count) != OK)
		return (FAIL);
	if (env_list_clone(&this->env_list, env_source) != OK)
		return (this->destroy(this), FAIL);
	this->pids = ft_calloc(this->cmd_count, sizeof(pid_t));
	if (this->pids == NULL)
		return (this->destroy(this), FAIL);
	return (OK);
}

static t_status	run_impl(t_proc_mgr *this, int *out_exit_status)
{
	t_cmd	*cur;
	int		i;

	cur = this->cmd_list->head;
	i = 0;
	while (i < this->cmd_count && cur != NULL)
	{
		if (proc_mgr_fork_one(this, cur, i) != OK)
			return (FAIL);
		cur = cur->next;
		i++;
	}
	this->io_mgr.close_all(&this->io_mgr);
	return (proc_mgr_wait_all(this, i, out_exit_status));
}

t_status	proc_mgr_init(t_proc_mgr *this, t_cmd_list *cmd_list,
		t_env_list *env_source, t_built_in *built_in)
{
	if (this == NULL || cmd_list == NULL || cmd_list->head == NULL
		|| env_source == NULL || built_in == NULL)
		return (FAIL);
	return (init_members(this, cmd_list, env_source, built_in));
}
