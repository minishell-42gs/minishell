/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   built_in.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tg <tg@student.42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/13 17:30:00 by tg                #+#    #+#             */
/*   Updated: 2026/09/13 17:30:00 by tg               ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "built_in.h"
#include <stddef.h>

static bool	is_built_in_impl(t_built_in *this, const char *cmd_name)
{
	(void)this;
	(void)cmd_name;
	return (false);
}

static t_status	run_impl(t_built_in *this, t_cmd *cmd,
		t_env_list *env_list, int *out_exit_status)
{
	(void)this;
	(void)cmd;
	(void)env_list;
	(void)out_exit_status;
	return (FAIL);
}

static void	destroy_impl(t_built_in *this)
{
	(void)this;
}

t_status	built_in_init(t_built_in *this)
{
	if (this == NULL)
		return (FAIL);
	this->is_built_in = is_built_in_impl;
	this->run = run_impl;
	this->destroy = destroy_impl;
	return (OK);
}
