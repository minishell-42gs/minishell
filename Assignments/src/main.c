/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: taegokim <taegokim@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/19 00:00:00 by hyuckwon          #+#    #+#             */
/*   Updated: 2026/10/11 15:25:43 by taegokim         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "app.h"
#include "libft.h"

int	main(int argc, char **argv, char **envp)
{
	t_app	app;

	(void)argc;
	(void)argv;
	ft_memset(&app, 0, sizeof(t_app));
	if (app_init(&app, envp) != OK)
		return (app.destroy(&app), 1);
	if (app.run(&app) != OK)
		return (app.destroy(&app), 1);
	app.destroy(&app);
	return (app.last_status);
}
