/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gomandam <gomandam@student.42madrid>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/24 02:02:59 by gomandam          #+#    #+#             */
/*   Updated: 2025/08/16 11:52:17 by gomandam         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"


// ? reference: https://github.com/g01100001/unattributed-source/blob/main/minishell-gonzlo-correction/src/built-ins/cd.c 

void	cd_error(char **argv[2])
{
	if ()
	//No argument (cd): goes to $HOME or errors if not set.
	//Single argument (cd dirname): checks existence, directory type, and calls chdir.
	//Error messages for missing HOME, not a directory, file not found.
}

// separate command logic from error handling, updates environment variables

int	cd(t_shell *shell, char *argv[])
{
	char	*cwd;
	
	if (!argv[1])
		return(printf("minishell: cd: error\n"), 0);
	if (chdir(argv[1]) != 0) 	// chdir function returns 0 on success and -1 on failure. Anything other than 0 is an error occurence.
		return (0);
	if (shell->prompt)		// ? 	TODO: retrospect. 
		free(shell->prompt);
	cwd = getcwd(NULL, 0);
	if (!cwd)
		return (0);
	
}
https://github.com/DanielCasti11o/Mini_Shell/blob/main/src/built_in.c

	// 1. Get command arguments
	// 2. Retrieve HOME from environment
	// 3. Build matrix with HOME
	// 4. Append current working directory to matrix
	// 5. Call error handler for cd logic and chdir
	// 6. If chdir successful, update OLDPWD
	// 7. Update PWD to new current directory
	// 8. Free and return
