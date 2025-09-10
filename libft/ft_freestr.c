
/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_exec1.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gomandam <gomandam@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/22 00:24:27 by gomandam          #+#    #+#             */
<<<<<<< HEAD:source/exec.c
/*   Updated: 2025/08/16 14:20:21 by gomandam         ###   ########.fr       */
=======
/*   Updated: 2025/09/09 22:06:58 by gomandam         ###   ########.fr       */
>>>>>>> origin/main:libft/ft_freestr.c
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_freestr(char **str)
{
	if (str != NULL && str != NULL)
	{
		free(*str);
		*str = NULL;
	}
	return (NULL);
}

int	exec_program(t_shell *shell)
{
}

