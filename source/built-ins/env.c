/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gomandam <gomandam@student.42madrid>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/24 02:03:25 by gomandam          #+#    #+#             */
/*   Updated: 2025/07/31 02:47:49 by gomandam         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// protoyped: char *envp[] or struct state t_minishell *shell
#include "minishell.h"

int	env(char *envp[])
{
	int	i;

	i = 0;
	while (envp && envp[i])		// check not NULL, iterates until NULL
	{
		if (ft_strchr(envp[i], '='))	// check '='
			printf("%s\n", envp[i]);	// std-out
		i++;				// next env array
	}
	return (0);
}

/*
	env - lists environmental variables outputs KEY=VALUE by child-process
	stores info used by processes (e.g. PATH, HOME, USER)
	each string has "=" so validate on this
	read (RTFM!) -> man 3 getenv
*/
