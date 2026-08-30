/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hyuckwon <hyuckwon@student.42gyeongsan.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/30 10:10:00 by hyuckwon          #+#    #+#             */
/*   Updated: 2026/08/30 10:10:00 by hyuckwon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "executor.h"

static t_status	run_impl(t_executor *this, t_cmd_list *cmd_list)
{
	(void)this;
	(void)cmd_list;
	return (OK);
}

static void	destroy_impl(t_executor *this)
{
	(void)this;
}

t_status	executor_init(t_executor *this, t_env_list *env_list)
{
	this->run = run_impl;
	this->destroy = destroy_impl;
	this->env_list = env_list;
	this->last_status = 0;
	return (OK);
}
