/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   app.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hyuckwon <hyuckwon@student.42gyeongsan.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/25 11:23:06 by hyuckwon          #+#    #+#             */
/*   Updated: 2026/07/25 14:51:29 by hyuckwon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "status.h"
#include "app.h"

t_status	app_init(t_app *this, int argc, char **argv, char **envp)
{
	(void)argc;
	(void)argv;
	this->envp = envp;
	this->last_status = 0;
	return (OK);
}
