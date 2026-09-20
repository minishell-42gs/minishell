/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tg <tg@student.42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/30 10:10:00 by hyuckwon          #+#    #+#             */
/*   Updated: 2026/09/13 16:28:13 by tg               ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "executor.h"
#include "proc_mgr.h"
#include "util.h"
#include <stddef.h>
#include <stdlib.h>

static t_status	run_external(t_executor *this, t_cmd_list *cmd_list,
		int *out_exit_status)
{
	char		**envp;
	t_proc_mgr	proc_mgr;
	t_status	status;

	envp = this->env_list->to_envp(this->env_list);
	if (envp == NULL)
		return (FAIL);
	if (proc_mgr_init(&proc_mgr, cmd_list, envp, &this->built_in) != OK)
		return (free_split(envp), FAIL);
	status = proc_mgr.run(&proc_mgr, out_exit_status);
	proc_mgr.destroy(&proc_mgr);
	free_split(envp);
	return (status);
}

static t_status	run_impl(t_executor *this, t_cmd_list *cmd_list,
		int *out_exit_status)
{
	if (cmd_list->head == NULL)
		return (OK);
	if (cmd_list->head->next == NULL
		&& this->built_in.is_built_in(&this->built_in,
			cmd_list->head->argv[0]))
		return (this->built_in.run(&this->built_in, cmd_list->head,
				this->env_list, out_exit_status));
	return (run_external(this, cmd_list, out_exit_status));
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
