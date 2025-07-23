/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strmapi.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: migugar2 <migugar2@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/23 19:47:50 by migugar2          #+#    #+#             */
/*   Updated: 2025/07/20 00:39:11 by migugar2         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strmapi(char const *s, char (*f)(unsigned int, char))
{
	char	*r;
	size_t	i;

	if (f == NULL || s == NULL)
		return (NULL);
	r = (char *)malloc(sizeof(char) * (ft_strlen(s) + 1));
	if (r == NULL)
		return (NULL);
	i = 0;
	while (s[i])
	{
		r[i] = f(i, s[i]);
		i++;
	}
	r[i] = '\0';
	return (r);
}
