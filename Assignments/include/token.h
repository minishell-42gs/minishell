/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: taegokim <taegokim@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/28 13:18:16 by taegokim          #+#    #+#             */
/*   Updated: 2026/07/29 15:38:27 by taegokim         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef TOKEN_H
# define TOKEN_H

# include "status.h"

typedef enum e_token_type		t_token_type;
typedef struct s_token			t_token;
typedef struct s_token_factory	t_token_factory;
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
t_status						token_init(t_token *this,
									t_token_type type, char *value);

struct							s_token_list
{
	t_token						*head;

	t_status					(*add_token)(t_token_list *this, t_token *node);
	void						(*destroy)(t_token_list *this);
};
t_status						token_list_init(t_token_list *this);

struct							s_token_factory
{
	t_token						*(*create)(t_token_factory *this,
								const char *str);
	void						(*destroy)(t_token_factory *this);
};
t_status						token_factory_init(t_token_factory *this);

#endif // TOKEN_H
