/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   built_in_util.h                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tg <tg@student.42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/04 15:00:00 by tg                #+#    #+#             */
/*   Updated: 2026/10/04 15:00:00 by tg               ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef BUILT_IN_UTIL_H
# define BUILT_IN_UTIL_H

# include "cmd.h"
# include "env.h"

int	builtin_echo(t_cmd *cmd);
int	builtin_cd(t_cmd *cmd, t_env_list *env);
int	builtin_pwd(t_cmd *cmd);
int	builtin_export(t_cmd *cmd, t_env_list *env);
int	builtin_unset(t_cmd *cmd, t_env_list *env);
int	builtin_env(t_cmd *cmd, t_env_list *env);
int	builtin_exit(t_cmd *cmd, int *status, bool *exit_requested);
int	builtin_error(const char *name, const char *detail);
int	builtin_arg_count(t_cmd *cmd);
int	builtin_valid_name(const char *name);

#endif
