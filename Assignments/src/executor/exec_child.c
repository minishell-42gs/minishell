/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_child.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tg <tg@student.42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/30 13:00:00 by hyuckwon          #+#    #+#             */
/*   Updated: 2026/09/12 14:58:22 by tg               ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cmd.h"
#include "error.h"
#include "executor.h"
#include <errno.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <unistd.h>

static void	exit_not_found(const char *name)
{
	t_error_req	req;
	int			child_status;

	req = (t_error_req){ERR_CMD_NOT_FOUND, 127, {.s_cmd_not_found = {name}}};
	error_report(&child_status, &req);
	exit(child_status);
}

static int	exec_error_status(int saved_errno)
{
	if (saved_errno == ENOENT || saved_errno == ENOTDIR)
		return (127);
	return (126);
}

static int	is_directory(const char *path)
{
	struct stat	stat_buf;

	if (stat(path, &stat_buf) != 0)
		return (0);
	return (S_ISDIR(stat_buf.st_mode));
}

static void	exit_exec_error(char *path, int saved_errno)
{
	t_error_req	req;
	int			child_status;

	child_status = exec_error_status(saved_errno);
	req = (t_error_req){ERR_ERRNO, child_status,
	{.s_sys = {path, saved_errno}}};
	error_report(&child_status, &req);
	free(path);
	exit(child_status);
}

void	exec_child(t_cmd *cmd, char **envp)
{
	char			*path;
	int				saved_errno;

	if (cmd->argv[0] == NULL)
		exit(0);
	path = create_cmd_path(cmd->argv[0], envp);
	if (path == NULL)
		exit_not_found(cmd->argv[0]);
	if (is_directory(path))
		exit_exec_error(path, EISDIR);
	execve(path, cmd->argv, envp);
	saved_errno = errno;
	exit_exec_error(path, saved_errno);
}
