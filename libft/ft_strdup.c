/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strdup.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: migugar2 <migugar2@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/19 18:58:41 by migugar2          #+#    #+#             */
/*   Updated: 2025/07/20 00:38:35 by migugar2         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strdup(const char *s)
{
	char	*s_dup;
	size_t	l;
	size_t	i;

	l = ft_strlen(s);
	s_dup = malloc(sizeof(char) * (l + 1));
	if (s_dup == NULL)
		return (NULL);
	i = 0;
	while (i < l)
	{
		s_dup[i] = (char)s[i];
		i++;
	}
	s_dup[i] = '\0';
	return (s_dup);
}
