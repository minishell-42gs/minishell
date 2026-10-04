/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tg <tg@student.42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/04 15:00:00 by tg                #+#    #+#             */
/*   Updated: 2026/10/04 15:00:00 by tg               ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "signals.h"
#include <readline/readline.h>
#include <signal.h>
#include <unistd.h>

static volatile sig_atomic_t	g_received_signal;

static void	catch_signal(int signal_number)
{
	g_received_signal = signal_number;
}

static int	readline_event(void)
{
	if (g_received_signal != SIGINT)
		return (0);
	rl_replace_line("", 0);
	rl_done = 1;
	write(STDOUT_FILENO, "\n", 1);
	rl_on_new_line();
	return (0);
}

/* readline must return on SIGINT so that both the prompt and a pending
 * heredoc can abort; the event hook sets rl_done instead of calling
 * readline functions from inside the signal handler. */
int	signals_install_prompt(void)
{
	struct sigaction	action;

	action = (struct sigaction){0};
	action.sa_handler = catch_signal;
	action.sa_flags = 0;
	sigemptyset(&action.sa_mask);
	rl_catch_signals = 0;
	if (isatty(STDIN_FILENO))
		rl_event_hook = readline_event;
	else
		rl_event_hook = NULL;
	rl_done = 0;
	if (sigaction(SIGINT, &action, NULL) == -1)
		return (-1);
	if (signal(SIGQUIT, SIG_IGN) == SIG_ERR)
		return (-1);
	return (0);
}

void	signals_ignore_execution(void)
{
	signal(SIGINT, SIG_IGN);
	signal(SIGQUIT, SIG_IGN);
}

int	signals_take(void)
{
	int	result;

	result = g_received_signal;
	g_received_signal = 0;
	rl_done = 0;
	return (result);
}
