/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: migugar2 <migugar2@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/20 00:00:07 by migugar2          #+#    #+#             */
/*   Updated: 2025/08/15 19:00:25 by migugar2         ###   ########.fr       */
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
		// TODO: Handle signals, and exit errors for return 1
		shell.line = readline(MINI_PROMPT);
		if (!shell.line)
			break ;
		if (*shell.line)
			add_history(shell.line);
		if (tokenize(shell.line, &shell.tokens) == 1)
		{
			ft_free((void **)&shell.line);
			break ;
		}
		if (shell.tokens == NULL)
		{
			ft_free((void **)&shell.line);
			continue ;
		}
		debug_tokenizer(shell.tokens);
		if (parse_ast(shell.tokens, &shell.ast) == 1)
		{
			// free_tokens(&shell.tokens);
			ft_free((void **)&shell.line);
			continue ;
		}
		debug_parser(shell.ast);
		free_ast_parse(&shell.ast); // ? Must use free_ast_final when ast is expanded
		// free_tokens(&tokens); // TODO: free tokens must not free because are freed
		ft_free((void **)&shell.line);
	}
	free_env_list(&shell.env_list);
	rl_clear_history();
}
