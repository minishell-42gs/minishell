/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: taegokim <taegokim@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/04 15:00:00 by taegokim          #+#    #+#             */
/*   Updated: 2026/10/11 15:18:26 by taegokim         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef HEREDOC_H
# define HEREDOC_H

# include "cmd.h"
# include "env.h"
# include "status.h"

t_status	heredoc_collect(t_cmd_list *cmds, t_env_list *env, int status,
				int *out_status);

#endif
