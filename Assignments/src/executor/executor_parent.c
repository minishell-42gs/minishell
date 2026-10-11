/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor_parent.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tg <tg@student.42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/04 15:00:00 by tg                #+#    #+#             */
/*   Updated: 2026/10/04 15:00:00 by tg               ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "executor.h"
#include "redirection.h"
#include <unistd.h>

static int	run_builtin(t_executor *this, t_cmd *cmd, int *out_status)
{
	t_builtin_result	result;

	result.status = *out_status;
	result.exit_requested = false;
	this->built_in.run(&this->built_in, cmd, this->env_list, &result);
	this->exit_requested = result.exit_requested;
	*out_status = result.status;
	return (result.status);
}

static void	restore_stdio(int saved_in, int saved_out)
{
	dup2(saved_in, STDIN_FILENO);
	dup2(saved_out, STDOUT_FILENO);
	close(saved_in);
	close(saved_out);
}

static int	apply_parent_redirections(t_cmd *cmd, int *status)
{
	if (cmd->redirs == NULL)
		return (OK);
	if (redirection_apply(cmd, status) != OK)
		return (FAIL);
	return (OK);
}

int	executor_run_parent(t_executor *this, t_cmd *cmd, int *out_status)
{
	int	saved_in;
	int	saved_out;
	int	status;

	saved_in = dup(STDIN_FILENO);
	saved_out = dup(STDOUT_FILENO);
	if (saved_in == -1 || saved_out == -1)
	{
		if (saved_in != -1)
			close(saved_in);
		if (saved_out != -1)
			close(saved_out);
		return (1);
	}
	status = *out_status;
	if (apply_parent_redirections(cmd, &status) == OK)
	{
		if (cmd->argv[0] != NULL)
			status = run_builtin(this, cmd, out_status);
	}
	*out_status = status;
	restore_stdio(saved_in, saved_out);
	return (status);
}
