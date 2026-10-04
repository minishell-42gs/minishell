/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_readline.c                                 :+:      :+:    :+:   */
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
#include <readline/readline.h>
#include <stdlib.h>
#include <unistd.h>

static t_status	grow_line(char **line, size_t *capacity, size_t length)
{
	char	*next;
	size_t	i;

	if (length + 1 < *capacity)
		return (OK);
	*capacity *= 2;
	if (*capacity < 64)
		*capacity = 64;
	next = ft_calloc(*capacity, sizeof(char));
	if (next == NULL)
		return (FAIL);
	i = 0;
	while (i < length)
	{
		next[i] = (*line)[i];
		i++;
	}
	free(*line);
	*line = next;
	return (OK);
}

static char	*read_line_fd(void)
{
	char	*line;
	char	c;
	size_t	capacity;
	size_t	length;
	ssize_t	count;

	line = NULL;
	capacity = 0;
	length = 0;
	while (1)
	{
		count = read(STDIN_FILENO, &c, 1);
		if (count < 0 && errno == EINTR)
			break ;
		if (count <= 0 || c == '\n')
			break ;
		if (grow_line(&line, &capacity, length) != OK)
			return (free(line), NULL);
		line[length++] = c;
	}
	if (count == 0 && length == 0)
		return (free(line), NULL);
	if (line == NULL)
		line = ft_calloc(1, sizeof(char));
	return (line);
}

char	*heredoc_read_prompt(void)
{
	if (isatty(STDIN_FILENO))
		return (readline("> "));
	return (read_line_fd());
}
