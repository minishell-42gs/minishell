/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tg <tg@student.42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/04 15:00:00 by tg                #+#    #+#             */
/*   Updated: 2026/10/04 15:00:00 by tg               ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "error.h"
#include "heredoc_internal.h"
#include "libft.h"
#include "signals.h"
#include <errno.h>
#include <signal.h>
#include <stdlib.h>
#include <unistd.h>

static void	report_eof(t_redir *redir)
{
	t_error_req	req;

	req = (t_error_req){ERR_HEREDOC_EOF, 0,
	{.s_heredoc_eof = {redir->target}}};
	error_report(NULL, &req);
}

static t_status	read_body(t_redir *redir, int fd,
		t_heredoc_context *context)
{
	char	*line;

	line = heredoc_read_prompt();
	while (line != NULL && ft_strncmp(line, redir->target,
			ft_strlen(redir->target) + 1) != 0)
	{
		if (signals_take() == SIGINT)
			return (free(line), *context->out_status = 130, FAIL);
		if (heredoc_write_line(fd, line, redir, context) != OK)
			return (free(line), FAIL);
		free(line);
		line = heredoc_read_prompt();
	}
	if (signals_take() == SIGINT)
		return (free(line), *context->out_status = 130, FAIL);
	if (line == NULL)
		report_eof(redir);
	free(line);
	return (OK);
}

static t_status	collect_one(t_redir *redir, t_heredoc_context *context)
{
	int	read_fd;
	int	write_fd;

	read_fd = heredoc_create_fd(&write_fd);
	if (read_fd == -1)
		return (*context->out_status = 1, FAIL);
	if (read_body(redir, write_fd, context) != OK)
		return (close(write_fd), close(read_fd), FAIL);
	close(write_fd);
	redir->hd_fd = read_fd;
	return (OK);
}

t_status	heredoc_collect(t_cmd_list *cmds, t_env_list *env, int status,
		int *out_status)
{
	t_heredoc_context	context;
	t_cmd				*cmd;
	t_redir				*redir;

	context = (t_heredoc_context){env, status, out_status};
	cmd = cmds->head;
	while (cmd != NULL)
	{
		redir = cmd->redirs;
		while (redir != NULL)
		{
			if (redir->type == REDIR_HEREDOC
				&& collect_one(redir, &context) != OK)
				return (FAIL);
			redir = redir->next;
		}
		cmd = cmd->next;
	}
	return (OK);
}
