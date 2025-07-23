/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fork-test.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gomandam <gomandam@student.42madrid>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/23 02:56:16 by gomandam          #+#    #+#             */
/*   Updated: 2025/07/23 03:38:12 by gomandam         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include <stdio.h>

int	main(int ac, char *av[])
{
	int	process_id;
	
	process_id = fork();
	if (process_id == 0)	//child process
	{
		usleep(100);
	}
	printf("Reference ID: %d && Parent ID: %d\n", getpid(), getppid());
	
	return(0);
}
