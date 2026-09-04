/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tg <tg@student.42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/19 13:59:08 by taegokim          #+#    #+#             */
/*   Updated: 2026/08/30 15:05:44 by tg               ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LEXER_H
# define LEXER_H

# include "status.h"
# include "token.h"

typedef struct s_lexer	t_lexer;

struct					s_lexer
{
	void				(*destroy)(t_lexer *this);
};
t_status				lexer_run(t_lexer *this, const char *line,
							t_token_list *tokens);
t_status				lexer_init(t_lexer *this);

/* internal */
t_status				lexer_tokenize(const char *line,
							t_token_list *token_list);
t_status				lexer_check_syntax(const char *line);

#endif // LEXER_H
