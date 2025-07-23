/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memmove.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: migugar2 <migugar2@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/17 20:30:38 by migugar2          #+#    #+#             */
/*   Updated: 2025/07/20 00:37:16 by migugar2         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static void	copy(char *dest, char *src, size_t n, int is_reverse)
{
	size_t	i;

	if (is_reverse)
	{
		i = n;
		while (i != 0)
		{
			i--;
			dest[i] = src[i];
		}
	}
	else
	{
		i = 0;
		while (i < n)
		{
			dest[i] = src[i];
			i++;
		}
	}
}

void	*ft_memmove(void *dest, const void *src, size_t n)
{
	char	*src_cpy;
	char	*dest_cpy;

	if (dest == NULL && src == NULL)
		return (NULL);
	src_cpy = (char *)src;
	dest_cpy = (char *)dest;
	if (src_cpy < dest_cpy && src_cpy + n > dest_cpy)
		copy(dest_cpy, src_cpy, n, 1);
	else
		copy(dest_cpy, src_cpy, n, 0);
	return (dest);
}
