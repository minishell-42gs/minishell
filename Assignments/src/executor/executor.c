/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tg <tg@student.42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/30 10:10:00 by hyuckwon          #+#    #+#             */
/*   Updated: 2026/10/04 15:00:00 by tg               ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "executor.h"
#include "signals.h"
#include "proc_mgr.h"
#include <stddef.h>

int	executor_run_parent(t_executor *this, t_cmd *cmd, int *out_status);

static t_status	run_external(t_executor *this, t_cmd_list *cmds,
		int *out_status)
{
	t_proc_mgr	proc_mgr;
	t_status	status;

	if (proc_mgr_init(&proc_mgr, cmds, this->env_list, &this->built_in) != OK)
		return (FAIL);
	signals_ignore_execution();
	status = proc_mgr.run(&proc_mgr, out_status);
	signals_install_prompt();
	proc_mgr.destroy(&proc_mgr);
	return (status);
}

static bool	run_in_parent(t_executor *this, t_cmd *cmd)
{
	if (cmd->next != NULL)
		return (false);
	if (cmd->argv[0] == NULL)
		return (cmd->redirs != NULL);
	return (this->built_in.is_built_in(&this->built_in, cmd->argv[0]));
}

static t_status	run_impl(t_executor *this, t_cmd_list *cmds,
		int *out_status)
{
	if (cmds->head == NULL)
		return (OK);
	this->exit_requested = false;
	if (run_in_parent(this, cmds->head))
	{
		*out_status = executor_run_parent(this, cmds->head, out_status);
		return (OK);
	}
	return (run_external(this, cmds, out_status));
}

static void	destroy_impl(t_executor *this)
{
	if (this == NULL)
		return ;
	if (this->built_in.destroy != NULL)
		this->built_in.destroy(&this->built_in);
}

t_status	executor_init(t_executor *this, t_env_list *env_list)
{
	if (this == NULL || env_list == NULL || env_list->to_envp == NULL)
		return (FAIL);
	if (built_in_init(&this->built_in) != OK)
		return (FAIL);
	this->run = run_impl;
	this->destroy = destroy_impl;
	this->env_list = env_list;
	return (OK);
}
