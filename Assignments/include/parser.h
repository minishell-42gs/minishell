/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: taegokim <taegokim@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/19 13:59:19 by taegokim          #+#    #+#             */
/*   Updated: 2026/07/29 16:41:23 by taegokim         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSER_H
# define PARSER_H

# include "cmd.h"
# include "status.h"
# include "token.h"

typedef struct s_parser	t_parser;

struct					s_parser
{
	t_cmd_factory		cmd_factory;

	void				(*destroy)(t_parser *this);
};
t_status				parser_run(t_parser *this, t_token *tokens_head,
							t_cmd_list *cmd_list);
t_status				parser_init(t_parser *this);

/* Internal parser helpers */
bool					parser_is_redirection(t_token_type type);
t_status				parser_add_redir(t_cmd *cmd, t_token *operator,
							t_token *target);
t_cmd					*parser_new_cmd(void);
t_status				parser_append_cmd(t_cmd_list *list, t_cmd *cmd);
t_status				parser_build(t_token *tokens, t_cmd_list *cmd_list);
t_status				parser_add_word(t_cmd *cmd, const char *value);

#endif // PARSER_H
