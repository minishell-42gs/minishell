/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error_system.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tg <tg@student.42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/12 16:00:00 by tg                #+#    #+#             */
/*   Updated: 2026/09/12 16:00:00 by tg               ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "error.h"
#include "libft.h"
#include <string.h>
#include <unistd.h>

void	error_print_system(const t_error_req *req)
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
