/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   app_child.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hyuckwon <hyuckwon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/11 14:00:00 by hyuckwon          #+#    #+#             */
/*   Updated: 2026/10/11 14:00:00 by hyuckwon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "app.h"
#include <readline/history.h>
#include <readline/readline.h>
#include <stdlib.h>

/*
 * Runs in a forked child that is about to exit without execve.
 * The child owns a copy of the parent's heap, so it releases the
 * command list, the input line and every app member before exiting.
 */
void	app_child_cleanup(void *ctx)
{
	t_app	*app;

	app = ctx;
	if (app == NULL)
		return ;
	if (app->cmds != NULL)
		app->cmds->destroy(app->cmds);
	app->cmds = NULL;
	free(app->line);
	app->line = NULL;
	app->destroy(app);
	rl_clear_history();
}
