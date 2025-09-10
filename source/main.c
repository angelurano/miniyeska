/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: migugar2 <migugar2@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/20 00:00:07 by migugar2          #+#    #+#             */
/*   Updated: 2025/09/10 02:23:07 by migugar2         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	main(int argc, char *argv[], char *envp[])
{
	t_shell	shell;

	ft_putstr_fd("Welcome to MiniYeska!\n", 1);
	(void)argc;
	(void)argv;
	if (init_shell(&shell, envp) == 1)
		return (1);
	while (1)
	{
		// TODO: Handle signals, and exit errors for return 1, create repl function
		shell.line = readline(MINI_PROMPT);
		if (!shell.line)
			break ;
		if (*shell.line)
			add_history(shell.line);
		if (tokenize(shell.line, &shell.tokens) == 1)
		{
			ft_freestr(&shell.line);
			break ;
		}
		if (shell.tokens == NULL)
		{
			ft_freestr(&shell.line);
			continue ;
		}
		// debug_tokenizer(&shell);
		if (parse_ast(shell.tokens, &shell.ast) == 1)
		{
			ft_freestr(&shell.line);
			continue ;
		}
		if (execute_ast(&shell, shell.ast) == 1)
		{
			// free_ast_parse(&shell.ast); // TODO: must free in execution
			ft_freestr(&shell.line);
			continue ;
		}
		// debug_parser(&shell);
		// free_ast_exp(&shell.ast); // TODO: must free in execution
		ft_freestr(&shell.line);
	}
	free_env_list(&shell.env_list);
	rl_clear_history();
}
