/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: taegokim <taegokim@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/30 10:00:00 by hyuckwon          #+#    #+#             */
/*   Updated: 2026/10/11 15:15:33 by taegokim         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXECUTOR_H
# define EXECUTOR_H

# include "built_in.h"
# include "cmd.h"
# include "env.h"
# include "proc_mgr.h"
# include "status.h"

typedef struct s_executor	t_executor;
typedef t_status			(*t_executor_run)(t_executor *this,
							t_cmd_list *cmd_list, int *out_exit_status);

struct						s_executor
{
	t_env_list				*env_list;
	t_built_in				built_in;
	bool					exit_requested;
	void					(*child_cleanup)(void *ctx);
	void					*cleanup_ctx;

	t_executor_run			run;
	void					(*destroy)(t_executor *this);
};

t_status					executor_init(t_executor *this,
								t_env_list *env_list);

#endif // EXECUTOR_H
