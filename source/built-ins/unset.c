/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gomandam <gomandam@student.42madrid>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/24 02:04:11 by gomandam          #+#    #+#             */
/*   Updated: 2025/08/05 04:33:56 by gomandam         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/*
	unset values & attribs of variables/functions
	remove access from current shell session, do not affect child process
*/

#include "../../include/minishell.h"

int	unset(char *argv[], char *envp[])
{
	int	a;
	int	b;
	char	*var;

	i = 0;
	if (!argv[0])
		return (0);

}

// Reference:	export(); function form my export.c
