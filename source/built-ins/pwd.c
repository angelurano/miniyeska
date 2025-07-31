/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gomandam <gomandam@student.42madrid>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/24 02:03:55 by gomandam          #+#    #+#             */
/*   Updated: 2025/07/31 02:10:34 by gomandam         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int	pwd(void)	// alternative prototype: state of session
{
	char *cwd;

	cwd = getcwd(NULL, 0);
	if (cwd == NULL)
	{
		perror("minishell: pwd: error retrieving current directory");
		return (0);
	}
	printf("%s\n", cwd);	// current working directory
	free(cwd);
	return (1);
}

// _getcwd	https://www.youtube.com/watch?v=n2iSn6zGB5A

/*
	getcwd() allocate buffer for current dir., check if null
	perror() error
*/
