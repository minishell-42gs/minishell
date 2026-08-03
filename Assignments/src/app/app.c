/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   app.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hyuckwon <hyuckwon@student.42gyeongsan.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/25 11:23:06 by hyuckwon          #+#    #+#             */
/*   Updated: 2026/08/02 17:14:17 by hyuckwon         ###   ########.fr       */
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
	t_cmd_list	*cmd_list;

	(void)this;
	cmd_list = ft_calloc(1, sizeof(t_cmd_list));
	if (cmd_list_init(cmd_list) != OK)
		return (FAIL);
	while (1)
	{
		line = readline("minishell$ ");
		if (!line)
			break ;
		if (is_blank(line) == false)
			add_history(line);
		parsing_facade_parse(this->parsing_facade, line, cmd_list, this->envp);
		free(line);
	}
	rl_clear_history();
	return (OK);
}

t_status	app_init(t_app *this, char **envp)
{
	this->envp = envp;
	this->last_status = 0;
	this->run = run_impl;

	return (OK);
}
