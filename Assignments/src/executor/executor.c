/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hyuckwon <hyuckwon@student.42gyeongsan.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/30 10:10:00 by hyuckwon          #+#    #+#             */
/*   Updated: 2026/08/30 13:30:00 by hyuckwon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "executor.h"
#include "util.h"
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>

/* Converts waitpid status into a shell exit code (bash convention). */
static int	wait_and_code(pid_t pid)
{
	int	status;

	if (waitpid(pid, &status, 0) == -1)
		return (1);
	if (WIFEXITED(status))
		return (WEXITSTATUS(status));
	if (WIFSIGNALED(status))
		return (128 + WTERMSIG(status));
	return (1);
}

/* v1: runs only the first command. Pipes come later. */
static t_status	run_impl(t_executor *this, t_cmd_list *cmd_list)
{
	char	**envp;
	pid_t	pid;

	if (cmd_list->head == NULL)
		return (OK);
	envp = this->env_list->to_envp(this->env_list);
	if (envp == NULL)
		return (FAIL);
	pid = fork();
	if (pid == -1)
		return (free_split(envp), FAIL);
	if (pid == 0)
		exec_child(cmd_list->head, envp);
	this->last_status = wait_and_code(pid);
	free_split(envp);
	return (OK);
}

static void	destroy_impl(t_executor *this)
{
	(void)this;
}

t_status	executor_init(t_executor *this, t_env_list *env_list)
{
	this->run = run_impl;
	this->destroy = destroy_impl;
	this->env_list = env_list;
	this->last_status = 0;
	return (OK);
}
