/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   app.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: taegokim <taegokim@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/25 13:02:05 by hyuckwon          #+#    #+#             */
/*   Updated: 2026/10/11 15:15:10 by taegokim         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef APP_H
# define APP_H
# include "env.h"
# include "executor.h"
# include "parsing_facade.h"
# include "status.h"

typedef struct s_app	t_app;

struct					s_app
{
	t_parsing_facade	parsing_facade;
	t_env_list			env_list;
	t_executor			executor;

	char				**envp;
	int					last_status;
	char				*line;
	t_cmd_list			*cmds;

	t_status			(*run)(t_app * this);
	void				(*destroy)(t_app *this);
};

t_status				app_init(t_app *this, char **envp);
void					app_child_cleanup(void *ctx);

#endif
