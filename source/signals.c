/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: migugar2 <migugar2@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/11 04:09:07 by migugar2          #+#    #+#             */
/*   Updated: 2025/09/11 22:32:58 by migugar2         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <signal.h>
#include "minishell.h"

volatile sig_atomic_t	g_signum = 0;

void	set_g_signum(int s)
{
	g_signum = s;
}

static void	handle_repl_sigint(int s)
{
	set_g_signum(s);
	write(STDOUT_FILENO, "\n", 1);
	rl_on_new_line();
	rl_replace_line("", 0);
	rl_redisplay();
}

void	signals_repl(t_shell *shell)
{
	struct sigaction	sa;

	if (!shell->interactive)
		return ;
	if (g_signum == SIGINT)
	{
		g_signum = 0;
		write(STDOUT_FILENO, "\n", 1);
	}
	ft_bzero(&sa, sizeof(sa));
	sa.sa_handler = handle_repl_sigint;
	sa.sa_flags = SA_RESTART;
	sigaction(SIGINT, &sa, NULL);
	signal(SIGQUIT, SIG_IGN);
}

void	signals_wait(t_shell *shell)
{
	struct sigaction	sa;

	if (!shell->interactive)
		return ;
	ft_bzero(&sa, sizeof(sa));
	sa.sa_handler = set_g_signum;
	sa.sa_flags = SA_RESTART;
	sigaction(SIGINT, &sa, NULL);
	signal(SIGQUIT, SIG_IGN);
}

void	signals_exec(t_shell *shell)
{
	if (!shell->interactive)
		return ;
	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_DFL);
}

/*
void	signals_heredoc(t_shell *shell)
{
	struct sigaction	sa;

	if (!shell->interactive)
		return ;
	ft_bzero(&sa, sizeof(sa));
	sa.sa_handler = set_g_signum;
	sa.sa_flags = SA_RESTART;
	sigaction(SIGINT, &sa, NULL);
	signal(SIGQUIT, SIG_IGN);
}
*/
