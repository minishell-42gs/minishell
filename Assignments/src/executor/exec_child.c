/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_child.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hyuckwon <hyuckwon@student.42gyeongsan.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/30 13:00:00 by hyuckwon          #+#    #+#             */
/*   Updated: 2026/08/30 22:55:24 by hyuckwon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cmd.h"
#include "executor.h"
#include "libft.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

/* Prints bash-style message to stderr and exits 127. Never returns. */
static void	exit_not_found(char *name)
{
	ft_putstr_fd("minishell: ", STDERR_FILENO);
	ft_putstr_fd(name, STDERR_FILENO);
	ft_putendl_fd(": command not found", STDERR_FILENO);
	exit(127);
}

/* Must be called in the forked child only. Never returns. */
void	exec_child(t_cmd *cmd, char **envp)
{
	char	*path;

	if (cmd->argv[0] == NULL)
		exit(0);
	path = create_cmd_path(cmd->argv[0], envp);
	if (path == NULL)
		exit_not_found(cmd->argv[0]);
	execve(path, cmd->argv, envp);
	perror("minishell");
	free(path);
	exit(126);
}
