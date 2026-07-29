/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: taegokim <taegokim@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/24 17:47:22 by hyuckwon          #+#    #+#             */
/*   Updated: 2026/07/29 15:01:16 by taegokim         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CMD_H
# define CMD_H

# include "status.h"
# include "token.h"
# include <signal.h>
# include <stdbool.h>

typedef struct s_redir			t_redir;
typedef struct s_cmd			t_cmd;
typedef struct s_cmd_list		t_cmd_list;
typedef struct s_cmd_factory	t_cmd_factory;

typedef enum e_redir_type
{
	REDIR_IN,
	REDIR_OUT,
	REDIR_APPEND,
	REDIR_HEREDOC
}								t_redir_type;

struct							s_redir
{
	t_redir_type				type;
	char						*target;
	bool						expand_body;
	int							hd_fd;
	t_redir						*next;
};

struct							s_cmd
{
	char						**argv;
	t_redir						*redirs;
	t_cmd						*next;

	t_status					(*append_argv)(t_cmd *this, char *str);
	void						(*destroy)(t_cmd *this);
};
// argv:{NULL} , redirs:NULL, next:NULL
t_status						cmd_init(t_cmd *this);

struct							s_cmd_list
{
	t_cmd						*head;

	t_status					(*add_cmd)(t_cmd_list *this, t_cmd *new_node);
	void						(*destroy)(t_cmd_list *this);
};
t_status						cmd_list_init(t_cmd_list *this);

struct							s_cmd_factory
{
	t_cmd						*(*create)(t_cmd_factory *this, t_token **token);
	void						(*destroy)(t_cmd_factory *this);
};
t_status						cmd_factory_init(t_cmd_factory *this);

// extern volatile sig_atomic_t	g_signal;
#endif
