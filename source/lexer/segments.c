/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   segments.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: migugar2 <migugar2@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/24 21:04:22 by migugar2          #+#    #+#             */
/*   Updated: 2025/07/24 21:36:03 by migugar2         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_seg	*new_seg(t_segtype k, const char *ptr, size_t len)
{
	t_seg	*seg;

	seg = malloc(sizeof(t_seg));
	if (!seg)
		return (NULL);
	seg->type = k;
	seg->slice.begin = ptr;
	seg->slice.len = len;
	seg->next = NULL;
	return (seg);
}

int	add_seg(t_lexer *lx, t_segtype type, const char *ptr, size_t len)
{
	t_seg	*seg;

	seg = new_seg(type, ptr, len);
	if (!seg)
		return (1);
	if (!lx->tok->seg_head)
		lx->tok->seg_head = seg;
	else
		lx->tok->seg_tail->next = seg;
	lx->tok->seg_tail = seg;
	return (0);
}
