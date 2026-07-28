/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_facade.h                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: taegokim <taegokim@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/26 11:08:07 by taegokim          #+#    #+#             */
/*   Updated: 2026/07/28 17:59:50 by taegokim         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSING_FACADE_H
# define PARSING_FACADE_H

# include "cmd.h"
# include "lexer.h"
# include "parser.h"
# include "status.h"

typedef struct s_parsing_facade	t_parsing_facade;

struct							s_parsing_facade
{
	t_lexer						lexer;
	t_parser					parser;

	void						(*destroy)(t_parsing_facade *this);
};
t_status						parsing_facade_parse(t_parsing_facade *this,
									const char *line, t_cmd **cmds,
									char **envp);

t_status						parsing_facade_init(t_parsing_facade *this);

#endif // PARSING_FACADE_H