/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_impl.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tg <tg@student.42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/04 15:00:00 by tg                #+#    #+#             */
/*   Updated: 2026/10/04 15:00:00 by tg               ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"
#include "libft.h"
#include <stdlib.h>

t_status	parser_add_word(t_cmd *cmd, const char *value)
{
	char	*raw;
	char	*word;

	raw = ft_strdup(value);
	word = ft_strdup(value);
	if (raw == NULL || word == NULL)
		return (free(raw), free(word), FAIL);
	if (cmd_append_raw_argv(cmd, raw) != OK)
		return (free(raw), free(word), FAIL);
	if (cmd_append_argv(cmd, word) != OK)
		return (free(word), FAIL);
	return (OK);
}

static t_status	process_redir(t_cmd *cmd, t_token **token)
{
	if ((*token)->next == NULL)
		return (FAIL);
	if (parser_add_redir(cmd, *token, (*token)->next) != OK)
		return (FAIL);
	*token = (*token)->next->next;
	return (OK);
}

static t_status	advance_pipe(t_cmd_list *list, t_cmd **cmd, t_token **token)
{
	if (parser_append_cmd(list, *cmd) != OK)
		return (FAIL);
	*cmd = parser_new_cmd();
	if (*cmd == NULL)
		return (FAIL);
	*token = (*token)->next;
	return (OK);
}

static t_status	process_token(t_cmd_list *list, t_cmd **cmd, t_token **token)
{
	if ((*token)->type == TOKEN_PIPE)
		return (advance_pipe(list, cmd, token));
	if (parser_is_redirection((*token)->type))
		return (process_redir(*cmd, token));
	if (parser_add_word(*cmd, (*token)->value) != OK)
		return (FAIL);
	*token = (*token)->next;
	return (OK);
}

t_status	parser_build(t_token *tokens, t_cmd_list *cmd_list)
{
	t_cmd	*cmd;
	t_token	*token;

	token = tokens;
	cmd = parser_new_cmd();
	if (cmd == NULL)
		return (FAIL);
	while (token != NULL)
	{
		if (process_token(cmd_list, &cmd, &token) != OK)
			return (cmd->destroy(cmd), free(cmd), FAIL);
	}
	return (parser_append_cmd(cmd_list, cmd));
}
