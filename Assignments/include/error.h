/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tg <tg@student.42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/06 18:16:41 by tg                #+#    #+#             */
/*   Updated: 2026/09/06 18:22:40 by tg               ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ERROR_H
# define ERROR_H

typedef enum e_error_type
{
	ERR_SYNTAX,
	ERR_CMD_NOT_FOUND,
	ERR_ERRNO,
	ERR_BUILTIN,
	ERR_AMBIGUOUS_REDIR,
	ERR_HEREDOC_EOF
}						t_error_type;

typedef struct s_error_req
{
	t_error_type		type;
	int					exit_code;
	union
	{
		struct
		{
			const char	*token;
		} s_syntax;
		struct
		{
			const char	*cmd;
		} s_cmd_not_found;
		struct
		{
			const char	*name;
			int			saved_errno;
		} s_sys;
		struct
		{
			const char	*name;
			const char	*detail;
		} s_builtin;
		struct
		{
			const char	*target;
		} s_ambiguous_redir;
		struct
		{
			const char	*delimiter;
		} s_heredoc_eof;
	} u_data;
}						t_error_req;

void					error_report(int *status, const t_error_req *req);

#endif // ERROR_H
