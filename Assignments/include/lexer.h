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
# include <stdbool.h>
# include <stddef.h>

typedef struct s_lexer	t_lexer;

struct					s_lexer
{
	void				(*destroy)(t_lexer *this);
};
t_status				lexer_run(t_lexer *this, const char *line,
							t_token_list *tokens, const char **syntax_token);
t_status				lexer_init(t_lexer *this);

/* internal */
typedef struct s_validation_state
{
	bool	segment;
	bool	pipe_seen;
	bool	expect_target;
}	t_validation_state;

t_status				lexer_tokenize(const char *line,
							t_token_list *token_list);
t_status				lexer_validate_tokens(t_token *tokens,
							const char **syntax_token);
size_t					lexer_token_length(const char *cursor);
t_token_type			lexer_classify_token_type(const char *value);
t_status				lexer_check_syntax(const char *line,
							const char **syntax_token);

#endif // LEXER_H
