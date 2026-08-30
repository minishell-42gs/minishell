/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   app.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tg <tg@student.42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/25 11:23:06 by hyuckwon          #+#    #+#             */
/*   Updated: 2026/08/15 10:08:52 by tg               ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "app.h"
#include "cmd.h"
#include "libft.h"
#include "status.h"
#include "util.h"
#include <readline/history.h>
#include <readline/readline.h>
#include <stdlib.h>

static t_status	process_line(t_app *this, const char *line)
{
	t_cmd_list	cmd_list;

	if (cmd_list_init(&cmd_list) != OK)
		return (FAIL);
	if (parsing_facade_parse(&this->parsing_facade, line, &cmd_list,
			this->envp) != OK)
		return (cmd_list.destroy(&cmd_list), FAIL);
	if (this->executor.run(&this->executor, &cmd_list) != OK)
		return (cmd_list.destroy(&cmd_list), FAIL);
	this->last_status = this->executor.last_status;
	cmd_list.destroy(&cmd_list);
	return (OK);
}

static t_status	run_impl(t_app *this)
{
	char	*line;

	while (1)
	{
		line = readline("minishell$ ");
		if (!line)
			break ;
		if (is_blank(line) == false)
		{
			add_history(line);
			if (process_line(this, line) != OK)
				return (free(line), rl_clear_history(), FAIL);
		}
		free(line);
	}
	rl_clear_history();
	return (OK);
}

static void	destroy_impl(t_app *this)
{
	if (this->executor.destroy != NULL)
		this->executor.destroy(&this->executor);
	if (this->env_list.destroy != NULL)
		this->env_list.destroy(&this->env_list);
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
	if (env_list_init(&this->env_list, envp) != OK)
		return (FAIL);
	if (executor_init(&this->executor, &this->env_list) != OK)
		return (FAIL);
	return (OK);
}
