/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error_redirection.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tg <tg@student.42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/12 16:00:00 by tg                #+#    #+#             */
/*   Updated: 2026/09/12 16:00:00 by tg               ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "error.h"
#include "libft.h"
#include <unistd.h>

void	error_print_redirection(const t_error_req *req)
{
	if (req == NULL)
		return ;
	ft_putstr_fd("minishell: ", STDERR_FILENO);
	ft_putstr_fd((char *)req->u_data.s_ambiguous_redir.target, STDERR_FILENO);
	ft_putendl_fd(": ambiguous redirect", STDERR_FILENO);
}

void	error_print_heredoc(const t_error_req *req)
{
	if (req == NULL)
		return ;
	ft_putstr_fd("minishell: warning: here-document ", STDERR_FILENO);
	ft_putstr_fd("delimited by end-of-file (wanted `", STDERR_FILENO);
	ft_putstr_fd((char *)req->u_data.s_heredoc_eof.delimiter, STDERR_FILENO);
	ft_putendl_fd("')", STDERR_FILENO);
}
