/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gomandam <gomandam@student.42madrid>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/24 02:04:11 by gomandam          #+#    #+#             */
/*   Updated: 2025/08/09 02:16:25 by gomandam         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/*
	unset values & attribs of variables/functions
	remove access from current shell session, do not affect child process
*/

#include "../../include/minishell.h"

int	unset(char *argv[], char *envp[])
{
	int		a;
	int		b;
	char	*var;

	a = 0;
	if (!argv[0])
		return (0);
	var = argv[1];
	while (envp[a])
	{
		if (ft_strncmp(envp[a], var, ft_strlen(var)) == 0
			&& envp[a][ft_strlen(var) == '='])
		{
			b = a;
			while (envp[b])
			{
				envp[b] = envp[b + 1];
				b++;
			}
			return (1);
		}
		a++;
	}
	printf("minishell: unset: variable %s not found\n", var);
	return (1);
}

// Reference:	export(); function form my export.c
