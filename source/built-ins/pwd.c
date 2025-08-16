/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gomandam <gomandam@student.42madrid>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/24 02:03:55 by gomandam          #+#    #+#             */
/*   Updated: 2025/08/16 16:35:44 by gomandam         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"
/*
** print working directory | current working directory
** Prints the current working directory to stdout.
** Returns 0 on success, 1 on failure.
	No need for t_shell args since !modify/read session state
*/
int	pwd(void)
{
	char	*cwd;

	cwd = getcwd(NULL, 0);
	if (cwd == NULL)
	{
		perror("minishell: pwd: error retrieving current directory");
		return (1);
	}
	printf("%s\n", cwd);
	free(cwd);
	return (0);
}

// _getcwd	https://www.youtube.com/watch?v=n2iSn6zGB5A
/*
	getcwd() allocate buffer for current dir., check if null
	perror() error
*/
