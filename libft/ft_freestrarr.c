/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_freestrarr.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: migugar2 <migugar2@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/10 02:05:46 by migugar2          #+#    #+#             */
/*   Updated: 2025/09/10 02:05:52 by migugar2         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_freestrarr(char ***arr)
{
	size_t	i;

	if (!arr || !*arr)
		return ;
	i = 0;
	while ((*arr)[i])
		free((*arr)[i++]);
	free(*arr);
	*arr = NULL;
}
