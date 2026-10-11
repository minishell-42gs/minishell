/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   create_cmd_path.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tg <tg@student.42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/03 18:21:30 by tg                #+#    #+#             */
/*   Updated: 2026/09/12 19:45:34 by tg               ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <errno.h>
#include <unistd.h>

static const char	*get_path_env(char **envp)
{
	int	i;

	if (envp == NULL)
		return (NULL);
	i = -1;
	while (envp[++i])
		if (ft_strncmp(envp[i], "PATH=", 5) == 0)
			return (envp[i] + 5);
	return (NULL);
}

static char	*join_path(const char *entry, size_t entry_len, const char *cmd)
{
	char	*dir;
	char	*dir_slash;
	char	*path;

	if (entry_len == 0)
		return (ft_strjoin("./", cmd));
	dir = ft_substr(entry, 0, entry_len);
	if (dir == NULL)
		return (NULL);
	dir_slash = ft_strjoin(dir, "/");
	free(dir);
	if (dir_slash == NULL)
		return (NULL);
	path = ft_strjoin(dir_slash, cmd);
	free(dir_slash);
	return (path);
}

static char	*check_path_entry(const char *entry, const char *cmd,
		char **denied_path)
{
	const char	*entry_end;
	char		*path;

	entry_end = ft_strchr(entry, ':');
	if (entry_end == NULL)
		entry_end = entry + ft_strlen(entry);
	path = join_path(entry, entry_end - entry, cmd);
	if (path == NULL)
		return (NULL);
	if (access(path, X_OK) == 0)
		return (path);
	if (errno == EACCES && *denied_path == NULL)
		*denied_path = path;
	else
		free(path);
	return (NULL);
}

static char	*find_cmd_path(const char *cmd, char **envp)
{
	const char	*path_env;
	const char	*entry;
	char		*path;
	char		*denied_path;

	path_env = get_path_env(envp);
	if (path_env == NULL)
		return (NULL);
	entry = path_env;
	denied_path = NULL;
	while (1)
	{
		path = check_path_entry(entry, cmd, &denied_path);
		if (path != NULL)
			return (free(denied_path), path);
		entry = ft_strchr(entry, ':');
		if (entry == NULL)
			break ;
		entry++;
	}
	return (denied_path);
}

char	*create_cmd_path(const char *cmd_name, char **envp)
{
	if (cmd_name == NULL || cmd_name[0] == '\0')
		return (NULL);
	if (ft_strchr(cmd_name, '/'))
		return (ft_strdup(cmd_name));
	return (find_cmd_path(cmd_name, envp));
}
