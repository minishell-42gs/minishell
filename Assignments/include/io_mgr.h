/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   io_mgr.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tg <tg@student.42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/13 17:00:00 by tg                #+#    #+#             */
/*   Updated: 2026/09/13 17:00:00 by tg               ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef IO_MGR_H
# define IO_MGR_H

# include "status.h"

typedef struct s_io_pipe	t_io_pipe;
typedef struct s_io_mgr		t_io_mgr;
typedef t_status			(*t_io_mgr_get_fds)(t_io_mgr *this, int index,
							int *in_fd, int *out_fd);
typedef void				(*t_io_mgr_close_all)(t_io_mgr *this);
typedef void				(*t_io_mgr_destroy)(t_io_mgr *this);

struct					s_io_pipe
{
	int					read_fd;
	int					write_fd;
};

struct					s_io_mgr
{
	t_io_pipe			*pipes;
	int					pipe_count;

	t_io_mgr_get_fds	get_fds;
	t_io_mgr_close_all	close_all;
	t_io_mgr_destroy	destroy;
};

t_status				io_mgr_init(t_io_mgr *this, int cmd_count);

#endif // IO_MGR_H
