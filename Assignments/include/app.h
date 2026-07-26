/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   app.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hyuckwon <hyuckwon@student.42gyeongsan.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/25 13:02:05 by hyuckwon          #+#    #+#             */
/*   Updated: 2026/07/25 14:44:10 by hyuckwon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef APP_H
# define APP_H
# include "status.h"

typedef struct s_app	t_app;

struct s_app
{
	char		**envp;
	int			last_status;
	t_status	(*run)(t_app * this);
	void		(*destroy)(t_app *this);
};

t_status	app_init(t_app *this, int argc, char **argv, char **envp);

#endif
