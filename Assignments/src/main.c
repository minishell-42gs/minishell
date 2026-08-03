/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hyuckwon <hyuckwon@student.42gyeongsan.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/19 00:00:00 by                   #+#    #+#             */
/*   Updated: 2026/08/02 15:08:31 by hyuckwon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include "app.h"
#include "libft.h"

int	main(int argc, char **argv, char **envp)
{
	t_app	app;

	(void)argc;
	(void)argv;
	ft_memset(&app, 0, sizeof(t_app));
	app_init(&app, envp);
	app.run(&app);
	printf("minishell: hello, we code\n");
	return (0);
}
