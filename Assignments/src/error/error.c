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
#include <stddef.h>

void	error_report(int *status, const t_error_req *req)
{
	if (req == NULL)
		return ;
	if (req->type == ERR_SYNTAX)
		error_print_syntax(req);
	else if (req->type == ERR_CMD_NOT_FOUND)
		error_print_command(req);
	else if (req->type == ERR_ERRNO)
		error_print_system(req);
	else if (req->type == ERR_BUILTIN)
		error_print_builtin(req);
	else if (req->type == ERR_AMBIGUOUS_REDIR)
		error_print_redirection(req);
	else if (req->type == ERR_HEREDOC_EOF)
		error_print_heredoc(req);
	else if (req->type == ERR_INTERNAL)
		error_print_internal(req);
	if (status != NULL)
		*status = req->exit_code;
}
