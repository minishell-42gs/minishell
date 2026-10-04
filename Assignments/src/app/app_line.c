/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   app_line.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tg <tg@student.42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/04 15:00:00 by tg                #+#    #+#             */
/*   Updated: 2026/10/04 15:00:00 by tg               ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "app.h"
#include "error.h"
#include "expansion.h"
#include "heredoc.h"
#include <stdbool.h>

static t_status	parse_command(t_app *this, const char *line,
		t_cmd_list *cmds, bool *should_run)
{
	t_parse_outcome	outcome;

	outcome = parsing_facade_parse(&this->parsing_facade, line, cmds);
	if (outcome.has_error_req)
		error_report(&this->last_status, &outcome.error);
	if (outcome.result == PARSE_SYNTAX_ERROR)
		*should_run = false;
	if (outcome.result == PARSE_FATAL_ERROR)
	{
		if (!outcome.has_error_req)
			this->last_status = 1;
		*should_run = false;
	}
	return (OK);
}

static t_status	expand_command(t_app *this, t_cmd_list *cmds,
		bool *should_run)
{
	const char	*ambiguous;
	t_error_req	req;

	if (expansion_apply(cmds, &this->env_list, this->last_status,
			&ambiguous) == OK)
		return (OK);
	if (ambiguous != NULL)
	{
		req = (t_error_req){ERR_AMBIGUOUS_REDIR, 1,
		{.s_ambiguous_redir = {ambiguous}}};
		error_report(&this->last_status, &req);
	}
	else
		this->last_status = 1;
	*should_run = false;
	return (OK);
}

static void	collect_heredocs(t_app *this, t_cmd_list *cmds,
		bool *should_run)
{
	if (heredoc_collect(cmds, &this->env_list, this->last_status,
			&this->last_status) == OK)
		return ;
	if (this->last_status != 130)
		this->last_status = 1;
	*should_run = false;
}

static void	execute_command(t_app *this, t_cmd_list *cmds)
{
	if (this->executor.run(&this->executor, cmds,
			&this->last_status) != OK)
		this->last_status = 1;
}

t_status	process_line(t_app *this, const char *line)
{
	t_cmd_list	cmds;
	bool		should_run;
	t_status	status;

	if (cmd_list_init(&cmds) != OK)
		return (FAIL);
	should_run = true;
	status = parse_command(this, line, &cmds, &should_run);
	if (status == OK && should_run)
		status = expand_command(this, &cmds, &should_run);
	if (status == OK && should_run)
		collect_heredocs(this, &cmds, &should_run);
	if (status == OK && should_run)
		execute_command(this, &cmds);
	cmds.destroy(&cmds);
	return (status);
}
