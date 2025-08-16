/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gomandam <gomandam@student.42madrid>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/24 02:03:44 by gomandam          #+#    #+#             */
/*   Updated: 2025/08/16 15:33:05 by gomandam         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

// TEMPORARY free_shell to be declared on minishel.h after being uploaded in branch
static void	free_shell(t_shell *shell)
{
	if (!shell)
		return;
	if (shell->line)
		free(shell->line);
	if (shell->tokens)
		free_tokens(&shell->tokens);
	if (shell->ast)
		free_ast_parse(&shell->ast);
	free_env_list(&shell->env_list);
}

//return 1 if string is valid, 0 otherwise.
static int	is_numeric(const char *str)
{
	int	i;

	i = 0;
	if (!str || !str[0])
		return (0);
	if (str[i] == '-' || str[i] == '+')
		i++;
	while (str[i])
	{
		if (str[i] < '0' || str[i] > '9')
			return (0);
		i++;
	}
	return (1);
}

int	exit(t_shell *shell, char *argv[])
{
	int	exit_status;

	exit_status = 0;
	printf("exit\n");
	if (argv[1])
	{
		if (!is_numeric(argv[1]))
		{
			printf("minishell: exit: %s: numeric argument required\n", argv[1]);
			exit_status = 255;
		}
		else if (argv[2])
		{
			printf("exit: too many arguments\n");
			return (1);
		}
		else
			exit_status = (unsigned char)ft_atoi(argv[1]);	// unsigned char to ensure UNIX exit codes [0, 255]
	}
	rl_clear_history();		// Free the readline history memory, since exit
	free_shell(shell);		// ! TO DO IMPLEMENT: Free all resources and structures
	exit(exit_status);
}

// For verbose BASH behaviour, use isatty(STDIN_FILENO); but exit() is enough
// This doesn't check for overflow or long long datatypes
