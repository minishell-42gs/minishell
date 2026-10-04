/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   proc_mgr.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tg <tg@student.42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/13 17:00:00 by tg                #+#    #+#             */
/*   Updated: 2026/09/20 18:34:26 by tg               ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PROC_MGR_H
# define PROC_MGR_H

# include "built_in.h"
# include "cmd.h"
# include "io_mgr.h"
# include "status.h"
# include <sys/types.h>

typedef struct s_proc_mgr	t_proc_mgr;
typedef t_status			(*t_proc_mgr_run)(t_proc_mgr *this,
				int *out_exit_status);

struct						s_proc_mgr
{
	t_env_list				env_list;
	t_io_mgr				io_mgr;
	pid_t					*pids;
	t_cmd_list				*cmd_list;
	int						cmd_count;
	t_built_in				*built_in;

	t_proc_mgr_run			run;
	void					(*destroy)(t_proc_mgr *this);
};

t_status					proc_mgr_init(t_proc_mgr *this,
								t_cmd_list *cmd_list, t_env_list *env_source,
								t_built_in *built_in);

/* internal */
int							proc_mgr_error(const char *name);
t_status					proc_mgr_fork_one(t_proc_mgr *this, t_cmd *cmd,
								int index);
t_status					proc_mgr_wait_all(t_proc_mgr *this, int child_count,
								int *out_exit_status);
t_status					proc_mgr_abort(t_proc_mgr *this, int child_count);
void						proc_mgr_exec(t_proc_mgr *this, t_cmd *cmd,
								int in_fd, int out_fd);
void						proc_mgr_exec_external(t_proc_mgr *this,
								t_cmd *cmd);
void						proc_mgr_exec_builtin(t_proc_mgr *this, t_cmd *cmd);

#endif // PROC_MGR_H
