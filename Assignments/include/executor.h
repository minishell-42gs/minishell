/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hyuckwon <hyuckwon@student.42gyeongsan.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/30 10:00:00 by hyuckwon          #+#    #+#             */
/*   Updated: 2026/08/30 09:16:59 by hyuckwon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXECUTOR_H
# define EXECUTOR_H

# include "cmd.h"
# include "env.h"
# include "status.h"

typedef struct s_executor	t_executor;

struct						s_executor
{
	t_env_list				*env_list;
	int						last_status;

	void					(*destroy)(t_executor *this);
};

t_status					executor_init(t_executor *this,
								t_env_list *env_list);

/* run is a plain function: norminette 3.3.50 cannot parse
 * a two-argument function pointer (see Code Convention) */
t_status					executor_run(t_executor *this,
								t_cmd_list *cmd_list);

/* child process only, never returns */
void						exec_child(t_cmd *cmd, char **envp);

#endif // EXECUTOR_H
