/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_file.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tg <tg@student.42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/04 15:00:00 by tg                #+#    #+#             */
/*   Updated: 2026/10/04 15:00:00 by tg               ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "heredoc_internal.h"
#include "libft.h"
#include <errno.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>

static char	*make_path(int id)
{
	char	*number;
	char	*path;

	number = ft_itoa(id);
	if (number == NULL)
		return (NULL);
	path = ft_strjoin("/tmp/.minishell-heredoc-", number);
	free(number);
	return (path);
}

static int	open_temp_path(char *path, int *write_fd)
{
	int	read_fd;

	*write_fd = open(path, O_CREAT | O_EXCL | O_WRONLY, 0600);
	if (*write_fd == -1)
		return (-1);
	read_fd = open(path, O_RDONLY);
	unlink(path);
	if (read_fd == -1)
		return (close(*write_fd), *write_fd = -1, -1);
	return (read_fd);
}

int	heredoc_create_fd(int *write_fd)
{
	char	*path;
	int		read_fd;
	int		id;

	id = 0;
	while (id < 100000)
	{
		path = make_path(id++);
		if (path == NULL)
			return (-1);
		read_fd = open_temp_path(path, write_fd);
		free(path);
		if (read_fd != -1 || errno != EEXIST)
			return (read_fd);
	}
	return (-1);
}
