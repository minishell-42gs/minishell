/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   app.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tg <tg@student.42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/25 13:02:05 by hyuckwon          #+#    #+#             */
/*   Updated: 2026/08/15 10:05:52 by tg               ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef APP_H
# define APP_H
# include "env.h"
# include "parsing_facade.h"
# include "status.h"

typedef struct s_app	t_app;

struct					s_app
{
	t_parsing_facade	parsing_facade;
	t_env_list			env_list;

	char				**envp;
	int					last_status;

	t_status			(*run)(t_app *this);
	void				(*destroy)(t_app *this);
};

t_status				app_init(t_app *this, char **envp);

#endif
