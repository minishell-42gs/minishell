/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_write.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tg <tg@student.42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/04 15:00:00 by tg                #+#    #+#             */
/*   Updated: 2026/10/04 15:00:00 by tg               ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "expansion.h"
#include "heredoc_internal.h"
#include "libft.h"
#include <stdlib.h>
#include <unistd.h>

static t_status	write_all(int fd, const char *value, size_t length)
{
	ssize_t	written;

	while (length > 0)
	{
		written = write(fd, value, length);
		if (written <= 0)
			return (FAIL);
		value += written;
		length -= written;
	}
	return (OK);
}

t_status	heredoc_write_line(int fd, const char *line, t_redir *redir,
		t_heredoc_context *context)
{
	char	*value;
	size_t	length;

	value = expansion_heredoc_line(line, context->env,
			context->status, redir->expand_body);
	if (value == NULL)
		return (FAIL);
	length = ft_strlen(value);
	if (write_all(fd, value, length) != OK
		|| write_all(fd, "\n", 1) != OK)
		return (free(value), FAIL);
	free(value);
	return (OK);
}
