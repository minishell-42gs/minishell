/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tg <tg@student.42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/06 18:24:47 by tg                #+#    #+#             */
/*   Updated: 2026/09/12 13:47:05 by tg               ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "error.h"
#include "libft.h"
#include <string.h>
#include <unistd.h>

static void	print_syntax(const t_error_req *req)
{
	if (req == NULL)
		return ;
	if (req->type == ERR_HEREDOC_EOF)
	{
		ft_putstr_fd("minishell: warning: here-document ", STDERR_FILENO);
		ft_putstr_fd("delimited by end-of-file (wanted `", STDERR_FILENO);
		ft_putstr_fd((char *)req->u_data.s_heredoc_eof.delimiter,
			STDERR_FILENO);
		ft_putendl_fd("')", STDERR_FILENO);
		return ;
	}
	ft_putstr_fd("minishell: syntax error near unexpected token `",
		STDERR_FILENO);
	ft_putstr_fd((char *)req->u_data.s_syntax.token, STDERR_FILENO);
	ft_putendl_fd("'", STDERR_FILENO);
}

static void	print_cmd_not_found(const t_error_req *req)
{
	if (req == NULL)
		return ;
	ft_putstr_fd("minishell: ", STDERR_FILENO);
	ft_putstr_fd((char *)req->u_data.s_cmd_not_found.cmd, STDERR_FILENO);
	ft_putendl_fd(": command not found", STDERR_FILENO);
}

static void	print_errno(const t_error_req *req)
{
	const char	*name;
	char		*detail;

	if (req == NULL)
		return ;
	name = req->u_data.s_sys.name;
	detail = strerror(req->u_data.s_sys.saved_errno);
	ft_putstr_fd("minishell: ", STDERR_FILENO);
	ft_putstr_fd((char *)name, STDERR_FILENO);
	ft_putstr_fd(": ", STDERR_FILENO);
	ft_putendl_fd(detail, STDERR_FILENO);
}

static void	print_builtin(const t_error_req *req)
{
	if (req == NULL)
		return ;
	ft_putstr_fd("minishell: ", STDERR_FILENO);
	if (req->type == ERR_AMBIGUOUS_REDIR)
	{
		ft_putstr_fd((char *)req->u_data.s_ambiguous_redir.target,
			STDERR_FILENO);
		ft_putendl_fd(": ambiguous redirect", STDERR_FILENO);
	}
	else
	{
		ft_putstr_fd((char *)req->u_data.s_builtin.name, STDERR_FILENO);
		ft_putstr_fd(": ", STDERR_FILENO);
		ft_putendl_fd((char *)req->u_data.s_builtin.detail, STDERR_FILENO);
	}
}

void	error_report(int *status, const t_error_req *req)
{
	if (req == NULL)
		return ;
	if (req->type == ERR_SYNTAX || req->type == ERR_HEREDOC_EOF)
		print_syntax(req);
	else if (req->type == ERR_CMD_NOT_FOUND)
		print_cmd_not_found(req);
	else if (req->type == ERR_ERRNO)
		print_errno(req);
	else if (req->type == ERR_BUILTIN || req->type == ERR_AMBIGUOUS_REDIR)
		print_builtin(req);
	if (status != NULL)
		*status = req->exit_code;
}
