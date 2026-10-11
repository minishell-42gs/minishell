/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_factory.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tg <tg@student.42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/04 15:00:00 by tg                #+#    #+#             */
/*   Updated: 2026/10/04 15:00:00 by tg               ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"
#include "libft.h"
#include <stdbool.h>
#include <stdlib.h>

bool	parser_is_redirection(t_token_type type)
{
	return (type == TOKEN_REDIR_IN || type == TOKEN_REDIR_OUT
		|| type == TOKEN_REDIR_APPEND || type == TOKEN_HEREDOC);
}

static t_redir_type	get_redir_type(t_token_type type)
{
	if (type == TOKEN_REDIR_IN)
		return (REDIR_IN);
	if (type == TOKEN_REDIR_APPEND)
		return (REDIR_APPEND);
	if (type == TOKEN_HEREDOC)
		return (REDIR_HEREDOC);
	return (REDIR_OUT);
}

t_status	parser_add_redir(t_cmd *cmd, t_token *operator, t_token *target)
{
	t_redir	*redir;
	t_redir	*tail;

	redir = ft_calloc(1, sizeof(t_redir));
	if (redir == NULL)
		return (FAIL);
	redir->type = get_redir_type(operator->type);
	redir->target = ft_strdup(target->value);
	redir->hd_fd = -1;
	if (redir->target == NULL)
		return (free(redir), FAIL);
	if (cmd->redirs == NULL)
		cmd->redirs = redir;
	else
	{
		tail = cmd->redirs;
		while (tail->next != NULL)
			tail = tail->next;
		tail->next = redir;
	}
	if (redir->type == REDIR_HEREDOC)
		redir->expand_body = (ft_strchr(target->value, '\'') == NULL
				&& ft_strchr(target->value, '"') == NULL);
	return (OK);
}

t_cmd	*parser_new_cmd(void)
{
	t_cmd	*cmd;

	cmd = ft_calloc(1, sizeof(t_cmd));
	if (cmd == NULL)
		return (NULL);
	if (cmd_init(cmd) != OK)
		return (free(cmd), NULL);
	return (cmd);
}

t_status	parser_append_cmd(t_cmd_list *list, t_cmd *cmd)
{
	if (cmd->raw_argv[0] == NULL && cmd->redirs == NULL)
		return (cmd->destroy(cmd), free(cmd), FAIL);
	if (cmd_list_add_cmd(list, cmd) != OK)
		return (cmd->destroy(cmd), free(cmd), FAIL);
	return (OK);
}
