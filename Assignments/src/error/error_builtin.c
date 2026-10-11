/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error_builtin.c                                    :+:      :+:    :+:   */
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

void	error_print_builtin(const t_error_req *req)
{
	if (req == NULL)
		return ;
	ft_putstr_fd("minishell: ", STDERR_FILENO);
	ft_putstr_fd((char *)req->u_data.s_builtin.name, STDERR_FILENO);
	ft_putstr_fd(": ", STDERR_FILENO);
	ft_putendl_fd((char *)req->u_data.s_builtin.detail, STDERR_FILENO);
}
