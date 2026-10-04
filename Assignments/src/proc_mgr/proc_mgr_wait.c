/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   proc_mgr_wait.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tg <tg@student.42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/13 17:30:00 by tg                #+#    #+#             */
/*   Updated: 2026/09/20 18:23:41 by tg               ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "error.h"
#include "proc_mgr.h"
#include <errno.h>
#include <sys/wait.h>
#include <unistd.h>

int	proc_mgr_error(const char *name)
{
	t_error_req	req;

	req = (t_error_req){ERR_ERRNO, 1, {.s_sys = {name, errno}}};
	error_report(NULL, &req);
	return (1);
}

static t_status	wait_one(pid_t pid, int *status)
{
	pid_t	result;

	result = waitpid(pid, status, 0);
	while (result == -1 && errno == EINTR)
		result = waitpid(pid, status, 0);
	if (result == -1)
		return (proc_mgr_error("waitpid"), FAIL);
	return (OK);
}

static int	exit_code(int status)
{
	if (WIFEXITED(status))
		return (WEXITSTATUS(status));
	if (WIFSIGNALED(status))
		return (128 + WTERMSIG(status));
	return (1);
}

t_status	proc_mgr_wait_all(t_proc_mgr *this, int child_count,
		int *out_exit_status)
{
	int			i;
	int			status;
	int			last_status;
	t_status	result;

	i = 0;
	result = OK;
	last_status = 1;
	while (i < child_count)
	{
		if (wait_one(this->pids[i], &status) != OK)
			result = FAIL;
		else if (i == child_count - 1)
			last_status = exit_code(status);
		i++;
	}
	if (result == OK && child_count > 0 && out_exit_status != NULL)
		*out_exit_status = last_status;
	return (result);
}

t_status	proc_mgr_abort(t_proc_mgr *this, int child_count)
{
	int	i;

	this->io_mgr.close_all(&this->io_mgr);
	i = 0;
	while (i < child_count)
	{
		kill(this->pids[i], SIGKILL);
		i++;
	}
	proc_mgr_wait_all(this, child_count, NULL);
	return (FAIL);
}
