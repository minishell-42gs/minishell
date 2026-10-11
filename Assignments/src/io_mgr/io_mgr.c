/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   io_mgr.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tg <tg@student.42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/13 15:30:00 by tg                #+#    #+#             */
/*   Updated: 2026/09/13 17:30:00 by tg               ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "error.h"
#include "io_mgr.h"
#include "libft.h"
#include <errno.h>
#include <stdlib.h>
#include <unistd.h>

static void	close_all_impl(t_io_mgr *this)
{
	int	i;

	if (this == NULL)
		return ;
	i = 0;
	while (i < this->pipe_count)
	{
		if (this->pipes[i].read_fd >= 0)
		{
			close(this->pipes[i].read_fd);
			this->pipes[i].read_fd = -1;
		}
		if (this->pipes[i].write_fd >= 0)
		{
			close(this->pipes[i].write_fd);
			this->pipes[i].write_fd = -1;
		}
		i++;
	}
}

static void	destroy_impl(t_io_mgr *this)
{
	if (this == NULL)
		return ;
	close_all_impl(this);
	free(this->pipes);
	this->pipes = NULL;
	this->pipe_count = 0;
}

static t_status	get_fds_impl(t_io_mgr *this, int index, int *in_fd,
		int *out_fd)
{
	if (this == NULL || in_fd == NULL || out_fd == NULL)
		return (FAIL);
	if (index < 0 || index > this->pipe_count)
		return (FAIL);
	if (index == 0)
		*in_fd = -1;
	else
		*in_fd = this->pipes[index - 1].read_fd;
	if (index == this->pipe_count)
		*out_fd = -1;
	else
		*out_fd = this->pipes[index].write_fd;
	return (OK);
}

static t_status	create_all_pipes(t_io_mgr *this)
{
	int			i;
	int			fds[2];
	t_error_req	req;

	i = 0;
	while (i < this->pipe_count)
	{
		if (pipe(fds) == -1)
		{
			req = (t_error_req){ERR_ERRNO, 1, {.s_sys = {"pipe", errno}}};
			error_report(NULL, &req);
			this->pipe_count = i;
			destroy_impl(this);
			return (FAIL);
		}
		this->pipes[i].read_fd = fds[0];
		this->pipes[i].write_fd = fds[1];
		i++;
	}
	return (OK);
}

t_status	io_mgr_init(t_io_mgr *this, int cmd_count)
{
	if (this == NULL || cmd_count <= 0)
		return (FAIL);
	this->destroy = destroy_impl;
	this->get_fds = get_fds_impl;
	this->close_all = close_all_impl;
	this->pipe_count = cmd_count - 1;
	this->pipes = NULL;
	if (this->pipe_count == 0)
		return (OK);
	this->pipes = ft_calloc(this->pipe_count, sizeof(t_io_pipe));
	if (this->pipes == NULL)
	{
		this->pipe_count = 0;
		return (FAIL);
	}
	return (create_all_pipes(this));
}
