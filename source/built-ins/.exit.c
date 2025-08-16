/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   .exit.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gomandam <gomandam@student.42madrid>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/24 02:03:44 by gomandam          #+#    #+#             */
/*   Updated: 2025/08/16 11:51:31 by gomandam         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

/*	Check string = valid numeric argument for exit
	Accepts optional + or - sign, then digits only	*/
static int	_numeric(const char *str)
{
	int	i;

	i = 0;
	if (!str || !str[0])
		return (0);
	if (str[0] == '-' || str[0] == '+')
		i++;
	while (str[i])
	{
		if (str[i] < '0' || str[i] > '9')
			return (0);
		i++;
	}
	return (1);
}
//	Converts string to long long
static long long	ft_atoll(const char *str)
{
	long long	res;
	int			sign;
	int			i;

	res = 0;
	sign = 1;
	i = 0;
	while (str[i] == ' ' || (str[i] >= 9 && str[i] <= 13))
		i++;
	if (str[i] == '-' || str[i] == '+')
	{
		if (str[i] == '-')
			sign = -1;
		i++;
	}
	while (str[i] >= '0' && str[i] <= '9')
	{
		res = res * 10 + (str[i] - '0');
		i++;
	}
	return (res * sign);
}

// ! TO-DO:	Implement freeing child processes: free_shell();
//		Or from miguel's free(); functions

/*	print "exit" before executing exit
 	handle: no args, 1 numeric, non-numeric & multiple
	set status variable for multiple args (no exit)
 */
int	exit(t_shell *shell, char *argv[])
{
	long long	status;

	printf("exit\n");
	if (!argv[1])
	{
		free_shell(&shell);	// implement free function
		exit(0);
	}
	if (!_numeric(argv[1]))
	{
		printf("minishell: exit: %s: numeric argument required\n", argv[1]);
		free_shell(&shell);
		exit(2);
	}
	if (argv[2])			// finalize this snippet
	{
		printf("minishell: exit: too many arguments\n");
		shell->status = 1;
		return ;
	}
	status = ft_atoll(argv[1]);
	status = exit((unsigned char)status);		// exit code valid range [0, 255] unsigned 8-bit
	if (status < 0)
		status += 256;
	free_shell(&shell);
	exit(status);
}
