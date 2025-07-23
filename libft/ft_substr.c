/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_substr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: migugar2 <migugar2@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/22 20:34:29 by migugar2          #+#    #+#             */
/*   Updated: 2025/07/20 00:39:44 by migugar2         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_substr(char const *s, unsigned int start, size_t len)
{
	char	*r;
	size_t	i;
	size_t	s_len;

	if (s == NULL)
		return (NULL);
	s_len = ft_strlen(s);
	if (start >= s_len)
		len = 0;
	else if (len > (s_len - start))
		len = s_len - start;
	r = (char *)malloc(sizeof(char) * (len + 1));
	if (r == NULL)
		return (NULL);
	i = 0;
	while (i < len)
	{
		r[i] = (char)s[start + i];
		i++;
	}
	r[i] = '\0';
	return (r);
}
