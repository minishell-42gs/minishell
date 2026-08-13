/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tg <tg@student.42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/08 14:27:19 by tg                #+#    #+#             */
/*   Updated: 2026/08/13 17:25:44 by tg               ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ENV_H
# define ENV_H
# include <status.h>
# include <stdbool.h>

typedef struct s_env_list	t_env_list;
typedef struct s_env		t_env;

struct						s_env
{
	char					*key;
	char					*value;
	t_env					*next;

	void					(*destroy)(t_env *this);
};
t_status					env_init(t_env *this, const char *key,
								const char *value);
t_env						*create_env(const char *key, const char *value);

struct						s_env_list
{
	t_env					*head;

	char					*(*get)(t_env_list *this, const char *key);
	char					**(*to_envp)(t_env_list *this);
	void					(*destroy)(t_env_list *this);
};
t_status					env_list_set(t_env_list *this, const char *key,
								const char *value);
t_status					env_list_unset(t_env_list *this, const char *key);
t_status					env_list_init(t_env_list *this, char **envp);

/* implement functions */
char						*env_list_get_impl(t_env_list *this,
								const char *key);
t_status					env_list_set_impl(t_env_list *this, const char *key,
								const char *value);
char						**env_list_to_envp_impl(t_env_list *this);
t_status					env_list_unset_impl(t_env_list *this,
								const char *key);

#endif // ENV_H
