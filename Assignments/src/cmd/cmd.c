/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: taegokim <taegokim@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/29 13:13:54 by taegokim          #+#    #+#             */
/*   Updated: 2026/07/29 14:59:44 by taegokim         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cmd.h"
#include "libft.h"
#include "util.h"
#include <stddef.h>
#include <stdlib.h>

static void	destroy_all_redirs(t_redir *redirs)
{
	t_redir	*temp;
	t_redir	*redir;

	redir = redirs;
	while (redir != NULL)
	{
		temp = redir->next;
		free(redir->target);
		free(redir);
		redir = temp;
	}
}

static void	destroy_impl(t_cmd *this)
{
	if (this->argv != NULL)
		free_split(this->argv);
	if (this->redirs != NULL)
		destroy_all_redirs(this->redirs);
}

t_status	cmd_append_argv(t_cmd *this, char *str)
{
	char	**new_argv;
	size_t	len;
	size_t	i;

	if (this->argv == NULL || str == NULL)
		return (FAIL);
	len = 0;
	while (this->argv[len] != NULL)
		len++;
	new_argv = ft_calloc(len + 2, sizeof(char *));
	if (!new_argv)
		return (FAIL);
	i = -1;
	while (++i < len)
		new_argv[i] = this->argv[i];
	new_argv[len] = str;
	new_argv[len + 1] = NULL;
	free(this->argv);
	this->argv = new_argv;
	return (OK);
}

t_status	cmd_init(t_cmd *this)
{
	this->destroy = destroy_impl;
	this->argv = ft_calloc(1, sizeof(char *));
	if (!this->argv)
		return (FAIL);
	this->redirs = NULL;
	this->next = NULL;
	return (OK);
}
