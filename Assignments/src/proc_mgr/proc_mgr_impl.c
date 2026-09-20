/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   proc_mgr_impl.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tg <tg@student.42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/13 17:30:00 by tg                #+#    #+#             */
/*   Updated: 2026/09/20 18:23:41 by tg               ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "proc_mgr.h"
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>

void	proc_mgr_exec_builtin(t_proc_mgr *this, t_cmd *cmd)
{
	int	exit_status;

	exit_status = 1;
	if (this->built_in->run(this->built_in, cmd, &this->env_list,
			&exit_status) == OK)
		exit(exit_status);
	exit(1);
}

void	proc_mgr_exec(t_proc_mgr *this, t_cmd *cmd, int in_fd,
		int out_fd)
{
	if (in_fd != -1 && in_fd != STDIN_FILENO)
	{
		if (dup2(in_fd, STDIN_FILENO) == -1)
			exit(1);
		close(in_fd);
	}
	if (out_fd != -1 && out_fd != STDOUT_FILENO)
	{
		if (dup2(out_fd, STDOUT_FILENO) == -1)
			exit(1);
		close(out_fd);
	}
	this->io_mgr.close_all(&this->io_mgr);
	if (this->built_in->is_built_in(this->built_in, cmd->argv[0]))
		proc_mgr_exec_builtin(this, cmd);
	else
		proc_mgr_exec_external(this, cmd);
}

t_status	proc_mgr_fork_one(t_proc_mgr *this, t_cmd *cmd, int index)
{
	int	in_fd;
	int	out_fd;

	in_fd = -1;
	out_fd = -1;
	if (this->io_mgr.get_fds(&this->io_mgr, index, &in_fd, &out_fd) != OK)
		return (proc_mgr_abort(this, index));
	this->pids[index] = fork();
	if (this->pids[index] == -1)
		return (proc_mgr_abort(this, index));
	if (this->pids[index] == 0)
		proc_mgr_exec(this, cmd, in_fd, out_fd);
	return (OK);
}

t_status	proc_mgr_wait_all(t_proc_mgr *this, int child_count,
		int *out_exit_status)
{
	int	i;
	int	status;
	int	result;

	i = 0;
	result = OK;
	while (i < child_count)
	{
		if (waitpid(this->pids[i], &status, 0) == -1)
			result = FAIL;
		else if (i == child_count - 1 && out_exit_status != NULL)
		{
			if (WIFEXITED(status))
				*out_exit_status = WEXITSTATUS(status);
			else if (WIFSIGNALED(status))
				*out_exit_status = 128 + WTERMSIG(status);
			else
				*out_exit_status = 1;
		}
		i++;
	}
	return (result);
}

t_status	proc_mgr_abort(t_proc_mgr *this, int child_count)
{
	this->io_mgr.close_all(&this->io_mgr);
	proc_mgr_wait_all(this, child_count, NULL);
	return (FAIL);
}
