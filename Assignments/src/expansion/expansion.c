/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansion.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tg <tg@student.42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/04 15:00:00 by tg                #+#    #+#             */
/*   Updated: 2026/10/04 15:00:00 by tg               ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "expansion_internal.h"
#include "libft.h"
#include "util.h"
#include <stdlib.h>

static t_status	expand_argv(t_cmd *cmd, t_expansion_context *context)
{
	char	**fields;
	size_t	count;
	size_t	i;

	free_split(cmd->argv);
	cmd->argv = ft_calloc(1, sizeof(char *));
	if (cmd->argv == NULL)
		return (FAIL);
	i = 0;
	while (cmd->raw_argv[i] != NULL)
	{
		fields = NULL;
		count = 0;
		if (exp_process_word(cmd->raw_argv[i], context, &fields, &count) != OK
			|| exp_append_fields(cmd, fields, count) != OK)
			return (free_split(fields), FAIL);
		free_split(fields);
		i++;
	}
	return (OK);
}

static t_status	expand_redirection(t_redir *redir,
		t_expansion_context *context, const char **ambiguous_target)
{
	char	*saved;
	char	**fields;
	size_t	count;

	saved = redir->target;
	if (redir->type == REDIR_HEREDOC)
		redir->target = strip_quotes(saved);
	else
	{
		fields = NULL;
		count = 0;
		if (exp_process_word(saved, context, &fields, &count) != OK
			|| count != 1)
			return (*ambiguous_target = saved, free_split(fields), FAIL);
		redir->target = fields[0];
		free(fields);
	}
	free(saved);
	if (redir->target == NULL)
		return (FAIL);
	return (OK);
}

static t_status	expand_redirections(t_cmd *cmd,
		t_expansion_context *context, const char **ambiguous_target)
{
	t_redir	*redir;

	redir = cmd->redirs;
	while (redir != NULL)
	{
		if (expand_redirection(redir, context, ambiguous_target) != OK)
			return (FAIL);
		redir = redir->next;
	}
	return (OK);
}

static t_status	expand_command(t_cmd *cmd, t_expansion_context *context,
		const char **ambiguous_target)
{
	if (expand_argv(cmd, context) != OK)
		return (FAIL);
	return (expand_redirections(cmd, context, ambiguous_target));
}

t_status	expansion_apply(t_cmd_list *cmd_list, t_env_list *env_list,
		int last_status, const char **ambiguous_target)
{
	t_cmd				*cmd;
	t_expansion_context	context;

	if (cmd_list == NULL || env_list == NULL || ambiguous_target == NULL)
		return (FAIL);
	*ambiguous_target = NULL;
	context.env_list = env_list;
	context.last_status = last_status;
	cmd = cmd_list->head;
	while (cmd != NULL)
	{
		if (expand_command(cmd, &context, ambiguous_target) != OK)
			return (FAIL);
		cmd = cmd->next;
	}
	return (OK);
}
