/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: taegokim <taegokim@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/19 13:59:19 by taegokim          #+#    #+#             */
/*   Updated: 2026/07/29 11:21:19 by taegokim         ###   ########.fr       */
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

	t_status			(*run)(t_parser *this, t_token *tokens_head,
					t_cmd_list *cmd_list);
	void				(*destroy)(t_parser *this);
};
t_status				parser_init(t_parser *this);

#endif // PARSER_H
