/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gomandam <gomandam@student.42madrid>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/24 02:02:59 by gomandam          #+#    #+#             */
/*   Updated: 2025/07/25 02:28:24 by gomandam         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void	cd_error(char **str[2])
{
	if ()
	//No argument (cd): goes to $HOME or errors if not set.
	//Single argument (cd dirname): checks existence, directory type, and calls chdir.
	//Error messages for missing HOME, not a directory, file not found.
}

// separate command logic from error handling, updates environment variables

int	cd(own_struct *placeholder)
{
	// 1. Get command arguments
	// 2. Retrieve HOME from environment
	// 3. Build matrix with HOME
	// 4. Append current working directory to matrix
	// 5. Call error handler for cd logic and chdir
	// 6. If chdir successful, update OLDPWD
	// 7. Update PWD to new current directory
	// 8. Free and return
}
