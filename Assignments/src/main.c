/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hyuckwon <hyuckwon@student.42gyeongsan.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/19 00:00:00 by                   #+#    #+#             */
/*   Updated: 2026/08/08 09:37:58 by hyuckwon         ###   ########.fr       */
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
