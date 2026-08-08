/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   app.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hyuckwon <hyuckwon@student.42gyeongsan.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/25 11:23:06 by hyuckwon          #+#    #+#             */
/*   Updated: 2026/08/08 09:48:42 by hyuckwon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "status.h"
#include "app.h"
#include <readline/readline.h>
#include <readline/history.h>
#include <stdlib.h>
#include "cmd.h"
#include "libft.h"
#include "util.h"

static t_status	run_impl(t_app *this)
{
	char		*line;
	t_cmd_list	cmd_list;

	while (1)
	{
		line = readline("minishell$ ");
		if (!line)
			break ;
		if (is_blank(line) == false)
		{
			add_history(line);
			if (cmd_list_init(&cmd_list) != OK)
				return (free(line), FAIL);
			parsing_facade_parse(&this->parsing_facade, line,
				&cmd_list, this->envp);
			cmd_list.destroy(&cmd_list);
		}
		free(line);
	}
	rl_clear_history();
	return (OK);
}

static void	destroy_impl(t_app *this)
{
	if (this->parsing_facade.destroy != NULL)
		this->parsing_facade.destroy(&this->parsing_facade);
}

t_status	app_init(t_app *this, char **envp)
{
	this->envp = envp;
	this->last_status = 0;
	this->run = run_impl;
	this->destroy = destroy_impl;
	if (parsing_facade_init(&this->parsing_facade) != OK)
		return (FAIL);
	return (OK);
}
