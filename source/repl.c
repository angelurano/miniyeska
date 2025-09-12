/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   repl.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: migugar2 <migugar2@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/11 01:33:59 by migugar2          #+#    #+#             */
/*   Updated: 2025/09/11 19:23:49 by migugar2         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*get_prompt(t_shell *shell)
{
	if (shell->interactive)
		return (MINI_PROMPT);
	return (NULL);
}

void	clear_repl(t_shell *shell)
{
	ft_freestr(&shell->line);
	free_env_list(&shell->env_list);
	rl_clear_history();
	if (shell->interactive)
		write(STDOUT_FILENO, "exit\n", 5);
}

void	set_last_status(t_shell *shell, long long status)
{
	if (shell)
		shell->last_status = (uint8_t)status;
}

void	repl(t_shell *shell)
{
	signals_repl(shell);
	while (1)
	{
		shell->line = readline(get_prompt(shell));
		if (g_signum == SIGINT)
		{
			g_signum = 0;
			set_last_status(shell, 130);
		}
		if (!shell->line)
			break ;
		if (shell->interactive && *shell->line)
			add_history(shell->line);
		if (tokenize(shell) == 1)
			break ;
		if (shell->tokens == NULL
			|| parse_ast(shell) == 1
			|| execute_ast(shell, shell->ast) == 1)
			ft_freestr(&shell->line); // TODO: must free in execution
		ft_freestr(&shell->line);
	}
	clear_repl(shell);
}
