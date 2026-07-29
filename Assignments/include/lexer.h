/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: taegokim <taegokim@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/19 13:59:08 by taegokim          #+#    #+#             */
/*   Updated: 2026/07/29 16:39:34 by taegokim         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LEXER_H
# define LEXER_H

# include "status.h"
# include "token.h"

typedef struct s_lexer	t_lexer;

struct					s_lexer
{
	t_token_factory		token_factory;

	void				(*destroy)(t_lexer *this);
};
t_status				lexer_run(t_lexer *this, const char *line,
							t_token_list *tokens);
t_status				lexer_init(t_lexer *this);

#endif // LEXER_H
