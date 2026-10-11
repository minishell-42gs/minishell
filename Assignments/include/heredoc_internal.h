/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_internal.h                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tg <tg@student.42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/04 15:00:00 by tg                #+#    #+#             */
/*   Updated: 2026/10/04 15:00:00 by tg               ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef HEREDOC_INTERNAL_H
# define HEREDOC_INTERNAL_H

# include "heredoc.h"

typedef struct s_heredoc_context
{
	t_env_list	*env;
	int			status;
	int			*out_status;
}			t_heredoc_context;

char		*heredoc_read_prompt(void);
int			heredoc_create_fd(int *write_fd);
t_status	heredoc_write_line(int fd, const char *line, t_redir *redir,
				t_heredoc_context *context);

#endif
