/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansion.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tg <tg@student.42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/04 15:00:00 by tg                #+#    #+#             */
/*   Updated: 2026/10/04 15:00:00 by tg               ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXPANSION_H
# define EXPANSION_H

# include "cmd.h"
# include "env.h"
# include "status.h"
# include <stdbool.h>

char				*strip_quotes(const char *raw);
char				*expansion_heredoc_line(const char *raw,
						t_env_list *env, int status, bool expand);
t_status			expansion_apply(t_cmd_list *cmd_list,
						t_env_list *env_list, int last_status,
						const char **ambiguous_target);

#endif
