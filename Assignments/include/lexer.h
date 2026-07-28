/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: taegokim <taegokim@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/19 13:59:08 by taegokim          #+#    #+#             */
/*   Updated: 2026/07/28 13:56:15 by taegokim         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LEXER_H
# define LEXER_H

# include "status.h"
# include "tokens.h"

typedef struct s_lexer	t_lexer;

struct					s_lexer
{
	t_status			(*run)(t_lexer *this, const char *line,
					t_tokens *tokens);
	void				(*destroy)(t_lexer *this);
};

t_status				lexer_init(t_lexer *this);

#endif // LEXER_H