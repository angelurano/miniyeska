/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memcpy.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: migugar2 <migugar2@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/16 20:41:06 by migugar2          #+#    #+#             */
/*   Updated: 2025/07/20 00:37:06 by migugar2         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memcpy(void *dest, const void *src, size_t n)
{
	unsigned char	*p;
	size_t			i;

	if (dest == NULL && src == NULL)
		return (NULL);
	p = (unsigned char *)dest;
	i = 0;
	while (i < n)
	{
		p[i] = ((unsigned char *)src)[i];
		i++;
	}
	return (dest);
}
