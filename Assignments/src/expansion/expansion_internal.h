/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansion_internal.h                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tg <tg@student.42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/04 15:00:00 by tg                #+#    #+#             */
/*   Updated: 2026/10/04 15:00:00 by tg               ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXPANSION_INTERNAL_H
# define EXPANSION_INTERNAL_H

# include "expansion.h"
# include <stdbool.h>
# include <stddef.h>

typedef struct s_word_state
{
	char	*value;
	size_t	length;
	size_t	capacity;
	char	**fields;
	size_t	count;
	bool	active;
}	t_word_state;

typedef struct s_expansion_context
{
	t_env_list	*env_list;
	int			last_status;
}	t_expansion_context;

bool		is_name_start(char c);
bool		is_name_char(char c);
t_status	exp_append_char(t_word_state *state, char c);
t_status	exp_append_field(t_word_state *state);
t_status	exp_append_text(t_word_state *state, const char *text, bool split);
t_status	exp_append_fields(t_cmd *cmd, char **fields, size_t count);
char		*exp_variable_value(const char *raw, size_t *index,
				t_expansion_context *context);
t_status	exp_process_word(const char *raw, t_expansion_context *context,
				char ***fields, size_t *count);

#endif
