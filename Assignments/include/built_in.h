/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   built_in.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tg <tg@student.42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/13 17:00:00 by tg                #+#    #+#             */
/*   Updated: 2026/10/04 15:00:00 by tg               ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef BUILT_IN_H
# define BUILT_IN_H

# include "cmd.h"
# include "env.h"
# include "status.h"
# include <stdbool.h>

typedef struct s_built_in		t_built_in;
typedef struct s_builtin_result	t_builtin_result;

struct					s_builtin_result
{
	int		status;
	bool	exit_requested;
};

typedef bool					(*t_built_in_is_built_in)(t_built_in *this,
							const char *cmd_name);
typedef t_status				(*t_built_in_run)(t_built_in *this, t_cmd *cmd,
							t_env_list *env_list, t_builtin_result *result);
typedef void					(*t_built_in_destroy)(t_built_in *this);

struct					s_built_in
{
	t_built_in_is_built_in	is_built_in;
	t_built_in_run			run;
	t_built_in_destroy		destroy;
};

t_status				built_in_init(t_built_in *this);

#endif
