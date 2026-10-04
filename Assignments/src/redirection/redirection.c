/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tg <tg@student.42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/04 15:00:00 by tg                #+#    #+#             */
/*   Updated: 2026/10/04 15:00:00 by tg               ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "error.h"
#include "redirection.h"
#include <errno.h>
#include <fcntl.h>
#include <unistd.h>

static int	open_redir(t_redir *redir)
{
	if (redir->type == REDIR_IN)
		return (open(redir->target, O_RDONLY));
	if (redir->type == REDIR_OUT)
		return (open(redir->target, O_WRONLY | O_CREAT | O_TRUNC, 0644));
	if (redir->type == REDIR_APPEND)
		return (open(redir->target, O_WRONLY | O_CREAT | O_APPEND, 0644));
	return (redir->hd_fd);
}

static int	target_fd(t_redir *redir)
{
	if (redir->type == REDIR_IN || redir->type == REDIR_HEREDOC)
		return (STDIN_FILENO);
	return (STDOUT_FILENO);
}

static void	report_redir_error(t_redir *redir, int saved_errno,
		int *out_status)
{
	t_error_req	req;

	*out_status = 1;
	req = (t_error_req){ERR_ERRNO, 1,
	{.s_sys = {redir->target, saved_errno}}};
	error_report(NULL, &req);
}

static t_status	apply_one(t_redir *redir, int *out_status)
{
	int	fd;
	int	errno_value;

	fd = open_redir(redir);
	if (fd == -1)
		return (report_redir_error(redir, errno, out_status), FAIL);
	if (dup2(fd, target_fd(redir)) == -1)
	{
		errno_value = errno;
		if (redir->type != REDIR_HEREDOC)
			close(fd);
		return (report_redir_error(redir, errno_value, out_status), FAIL);
	}
	if (redir->type == REDIR_HEREDOC)
		redir->hd_fd = -1;
	else
		close(fd);
	return (OK);
}

t_status	redirection_apply(t_cmd *cmd, int *out_status)
{
	t_redir	*redir;

	if (cmd == NULL || out_status == NULL)
		return (FAIL);
	redir = cmd->redirs;
	while (redir != NULL)
	{
		if (apply_one(redir, out_status) != OK)
			return (FAIL);
		redir = redir->next;
	}
	*out_status = 0;
	return (OK);
}
