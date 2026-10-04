/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   app.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tg <tg@student.42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/25 11:23:06 by hyuckwon          #+#    #+#             */
/*   Updated: 2026/10/04 15:00:00 by tg               ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "app.h"
#include "signals.h"
#include "util.h"
#include <readline/history.h>
#include <readline/readline.h>
#include <signal.h>
#include <stdlib.h>

t_status	process_line(t_app *this, const char *line);

static t_status	read_prompt_line(t_app *this, char **line, bool *interrupted)
{
	if (signals_install_prompt() != 0)
		return (FAIL);
	*line = readline("minishell$ ");
	*interrupted = (signals_take() == SIGINT);
	if (*interrupted)
	{
		free(*line);
		*line = NULL;
		this->last_status = 130;
	}
	return (OK);
}

static t_status	process_input_line(t_app *this, char *line)
{
	if (is_blank(line))
		return (OK);
	add_history(line);
	return (process_line(this, line));
}

static t_status	run_impl(t_app *this)
{
	char	*line;
	bool	interrupted;

	while (1)
	{
		if (read_prompt_line(this, &line, &interrupted) != OK)
			return (FAIL);
		if (interrupted)
			continue ;
		if (line == NULL)
			break ;
		if (process_input_line(this, line) != OK)
			return (free(line), rl_clear_history(), FAIL);
		free(line);
		if (this->executor.exit_requested)
			break ;
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
	if (env_list_init(&this->env_list, envp) != OK)
		return (FAIL);
	if (parsing_facade_init(&this->parsing_facade, &this->env_list) != OK)
		return (FAIL);
	if (executor_init(&this->executor, &this->env_list) != OK)
		return (FAIL);
	return (OK);
}
