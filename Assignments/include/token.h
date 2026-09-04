/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tg <tg@student.42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/28 13:18:16 by taegokim          #+#    #+#             */
/*   Updated: 2026/08/26 11:05:20 by tg               ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef TOKEN_H
# define TOKEN_H

# include "status.h"

typedef enum e_token_type		t_token_type;
typedef struct s_token			t_token;
typedef struct s_token_list		t_token_list;

enum							e_token_type
{
	TOKEN_WORD,
	TOKEN_PIPE,
};

struct							s_token
{
	t_token_type				type;
	char						*value;
	t_token						*next;

	void						(*destroy)(t_token *this);
};
t_status						token_init(t_token *this, t_token_type type,
									char *value);

struct							s_token_list
{
	t_token						*head;
	t_token						*tail;

	void						(*destroy)(t_token_list *this);
};
t_status						token_list_add_token(t_token_list *this,
									t_token *node);
t_status						token_list_init(t_token_list *this);

#endif // TOKEN_H
