/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gomandam <gomandam@student.42madrid>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/24 02:03:44 by gomandam          #+#    #+#             */
/*   Updated: 2025/08/03 05:06:24 by gomandam         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

static int	is_numeric(const char *str)
{
	int	i;

	i = 0;
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

int	exit(t_minishell *shell, char *argv[])
{
	int	exit_status;

	exit_status = 0;
	if (argv[1])
	{
		if (!is_numeric(argv[1]))
		{
			printf("exit: numeric argument required\n");
			exit_status = 255;
		}
		else if (argv[2])
		{
			printf("exit: too many arguments\n");	// alternative: write(2, "stderr", buff);
			return (1);
		}
		else
			exit_status = atoi(argv[1]) % 256;	// UNIX exit codes [0, 255]
	}
	rl_clear_history();		// Free the readline history memory, since exit
	free_shell(shell);		// ! IMPLEMENT: Free all resources and structures
	exit(exit_status);
}

// For verbose BASH behaviour, use isatty(STDIN_FILENO); but exit() is enough
