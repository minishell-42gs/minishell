/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: taegokim <taegokim@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/19 13:59:19 by taegokim          #+#    #+#             */
/*   Updated: 2026/07/29 09:25:07 by taegokim         ###   ########.fr       */
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
	t_status			(*run)(t_parser *this, const t_token_list *tokens,
					t_cmd **cmds);
	void				(*destroy)(t_parser *this);
};

t_status				parser_init(t_parser *this);

#endif // PARSER_H
