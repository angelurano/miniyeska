/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   alternatives.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: migugar2 <migugar2@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/12 18:36:09 by migugar2          #+#    #+#             */
/*   Updated: 2025/08/16 17:44:49 by migugar2         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*literal_expansion(t_tok *word)
{
	size_t		len;
	const char	*last_char;

	last_char = word->seg_tail->slice.begin + word->seg_tail->slice.len;
	len = last_char - word->seg_head->slice.begin;
	return (ft_strndup(word->seg_head->slice.begin, len));
}

static size_t	get_len_simple(t_tok *word)
{
	size_t	len;
	t_seg	*cur;

	len = 0;
	cur = word->seg_head;
	while (cur != NULL)
	{
		if (cur->type == SEG_TEXT)
			len += cur->slice.len;
		else if (cur->type == SEG_PARAM)
			len += cur->slice.len + 1;
		else if (cur->type == SEG_WILDCARD)
			len++;
		cur = cur->next;
	}
	return (len);
}

static void	cpy_simple(t_tok *word, char *expanded)
{
	t_seg	*cur;
	size_t	i;

	cur = word->seg_head;
	i = 0;
	while (cur != NULL)
	{
		if (cur->type == SEG_PARAM)
			expanded[i++] = '$';
		if (cur->type == SEG_TEXT || cur->type == SEG_PARAM)
		{
			ft_strlcpy(expanded + i, cur->slice.begin, cur->slice.len + 1);
			i += cur->slice.len;
		}
		else if (cur->type == SEG_WILDCARD)
			expanded[i++] = '*';
		cur = cur->next;
	}
	expanded[i] = '\0';
}

// TODO: Apply in heredoc word
char	*simple_expansion(t_tok *word)
{
	size_t	len;
	char	*expanded;

	len = get_len_simple(word);
	expanded = malloc(sizeof(char) * (len + 1));
	if (expanded == NULL)
		return (NULL);
	cpy_simple(word, expanded);
	return (expanded);
}
