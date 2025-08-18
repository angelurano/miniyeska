/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gomandam <gomandam@student.42madrid>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/24 02:03:25 by gomandam          #+#    #+#             */
/*   Updated: 2025/08/18 23:33:26 by gomandam         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// protoyped: char *envp[] or struct state t_shell *shell
#include "../../include/minishell.h"

int	env(t_env_list *env_list)
{
	t_env	*current;

	current = env_list->head;
	while (current)
	{
		if (current->full && ft_strchr(current->full, '='))
			printf("%s\n", current->full);
		current = current->next;
	}
	return (0);
}

/*
	env - lists environmental variables outputs KEY=VALUE by child-process
	stores info used by processes (e.g. PATH, HOME, USER)
	each string has "=" so validate on this
	read (RTFM!) -> man 3 getenv
*/
