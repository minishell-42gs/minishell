/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hyuckwon <hyuckwon@student.42gyeongsan.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/24 17:47:22 by hyuckwon          #+#    #+#             */
/*   Updated: 2026/07/24 17:55:27 by hyuckwon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CMD_H
# define CMD_H

# include <stdbool.h>
# include <signal.h>

typedef struct s_redir			t_redir;
typedef struct s_cmd			t_cmd;

typedef enum e_redir_type
{
	REDIR_IN,
	REDIR_OUT,
	REDIR_APPEND,
	REDIR_HEREDOC
}								t_redir_type;

struct s_redir
{
	t_redir_type	type;
	char			*target;
	bool			expand_body;
	int				hd_fd;
	t_redir			*next;
};

struct s_cmd
{
	char	**argv;
	t_redir	*redirs;
	t_cmd	*next;
};

// extern volatile sig_atomic_t	g_signal;
#endif 
