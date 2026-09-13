/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_facade.h                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tg <tg@student.42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/26 11:08:07 by taegokim          #+#    #+#             */
/*   Updated: 2026/08/30 14:19:56 by tg               ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSING_FACADE_H
# define PARSING_FACADE_H

# include "cmd.h"
# include "env.h"
# include "error.h"
# include "lexer.h"
# include "parser.h"
# include "status.h"
# include <stdbool.h>

typedef struct s_parsing_facade	t_parsing_facade;

typedef enum e_parse_result
{
	PARSE_OK,
	PARSE_SYNTAX_ERROR,
	PARSE_FATAL_ERROR
}	t_parse_result;

typedef struct s_parse_outcome
{
	t_parse_result	result;
	bool			has_error_req;
	t_error_req		error;
}	t_parse_outcome;

struct							s_parsing_facade
{
	t_lexer						lexer;
	t_parser					parser;
	t_env_list					*env_list;

	void						(*destroy)(t_parsing_facade *this);
};
t_parse_outcome					parsing_facade_parse(t_parsing_facade *this,
									const char *line, t_cmd_list *cmd_list);
t_status						parsing_facade_init(t_parsing_facade *this,
									t_env_list *env_list);

#endif // PARSING_FACADE_H
