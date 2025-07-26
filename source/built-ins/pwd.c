/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gomandam <gomandam@student.42madrid>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/24 02:03:55 by gomandam          #+#    #+#             */
/*   Updated: 2025/07/25 03:04:43 by gomandam         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int	pwd(own_struct	*shell) // alternative prototype: pwd(void)
{
	char *cwd;

	cwd = getcwd(NULL, 0);

	// error checking
	if (cwd == NULL)
	{
		perror("minishell: pwd: error");
		return (error_code);
	}
	printf("CURRENT WORKING DIRECTORY: / / /");
	free(cwd);
	return (1);
}

// _getcwd	https://www.youtube.com/watch?v=n2iSn6zGB5A
