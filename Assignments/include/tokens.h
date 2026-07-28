/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokens.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: taegokim <taegokim@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/28 13:18:16 by taegokim          #+#    #+#             */
/*   Updated: 2026/07/28 17:45:10 by taegokim         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef TOKENS_H
# define TOKENS_H

# include "status.h"

typedef enum e_token_type		t_token_type;
typedef struct s_token_node		t_token_node;
typedef struct s_token_factory	t_token_factory;
typedef struct s_tokens			t_tokens;

enum							e_token_type
{
	TOKEN_WORD,
};

struct							s_token_node
{
	t_token_type				type;
	char						*value;
	t_token_node				*next;

	void						(*destroy)(t_token_node *this);
};
t_status						token_node_init(t_token_node *this,
									t_token_type type, char *value);

struct							s_token_factory
{
	t_token_node				*(*create)(t_token_factory *this,
						const char *str);
	void						(*destroy)(t_token_factory *this);
};
t_status						token_factory_init(t_token_factory *this);

struct							s_tokens
{
	t_token_node				*tokens;
	t_token_factory				token_factory;

	void						(*add_token)(t_tokens *this,
								t_token_node *node);
	void						(*destroy)(t_tokens *this);
};
t_status						tokens_init(t_tokens *this);

#endif // TOKENS_H