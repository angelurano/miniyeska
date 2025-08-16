/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gomandam <gomandam@student.42madrid>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/24 02:03:14 by gomandam          #+#    #+#             */
/*   Updated: 2025/08/16 14:03:16 by gomandam         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

static int	echo_flag(char *argv)		// handles -n or -nnnn for newlines
{
	int	i;

	if (!argv || argv[0] != '-' || argv[1] != 'n')		// if argv != -n
		return (0);
	i = 2;
	while (argv[i] != '\0')
	{
		if (argv[i] != 'n')		// -n flag
			return (0);
		i++;
	}
	return (1);
}

static void	echo_print(char *argv[], int start)
{
	int	i;

	i = start;
	while (argv[i] != NULL)	 //instead '\0', null pointer not string/char termination
	{
		printf("%s", argv[i]);		// print arguments
		if (argv[i + 1] != NULL)
			printf(" ");		// space after each argv, unless NULL
		i++;
	}
}

/*	Minishell built-in: echo
 *	argv[0] = "echo", argv[1..] = arguments
 *	Supports multiple -n flags: echo -n -n hello */

int	echo(char *argv[])
{
	int	i;
	int	n_flag;

	i = 1;
	n_flag = 0;
	while (argv[i] && echo_flag(argv[i]))		// check multiple -n flags
	{
		n_flag = 1;
		i++;
	}
	if (argv[i])			// print arguments
		echo_print(argv, i);
	if (!n_flag)			// newline if no -n flag
		printf("\n");
	return (0);
}
