/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handlers.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: migugar2 <migugar2@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/24 21:09:42 by migugar2          #+#    #+#             */
/*   Updated: 2025/08/08 18:43:41 by migugar2         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_lxstate	handle_in_single_q(t_lexer *lx)
{
	const char	*start;

	start = lx->cur;
	while (*lx->cur && *lx->cur != '\'')
		lx_advance(lx);
	if (*lx->cur == '\0')
		return (LX_DIE);
	if (start_word(lx) == 1)
		return (LX_ERR);
	if (add_seg(lx, SEG_TEXT, start, lx->cur - start) == 1)
		return (LX_ERR);
	lx_advance(lx);
	return (LX_GENERAL);
}

t_lxstate	handle_in_double_q(t_lexer *lx)
{
	const char	*start;

	start = lx->cur;
	while (*lx->cur && *lx->cur != '\"')
	{
		if (*lx->cur == '$' && (lx->cur[1] == '?'
				|| lx->cur[1] == '_' || ft_isalnum(lx->cur[1])))
			break ;
		lx_advance(lx);
	}
	if (*lx->cur == '\0')
		return (LX_DIE);
	if (start_word(lx) == 1)
		return (LX_ERR);
	if (add_seg(lx, SEG_TEXT, start, lx->cur - start) == 1)
		return (LX_ERR);
	if (*lx->cur == '$')
		return (lx_advance(lx), LX_PARAM);
	lx_advance(lx);
	return (LX_GENERAL);
}

t_lxstate	handle_param(t_lexer *lx)
{
	const char	*start;

	start = lx->cur;
	if (*lx->cur == '?')
		lx_advance(lx);
	else
	{
		while (*lx->cur && (ft_isalnum(*lx->cur) || *lx->cur == '_'))
			lx_advance(lx);
	}
	if (add_seg(lx, SEG_PARAM, start, lx->cur - start) == 1)
		return (LX_ERR);
	return (lx->prev);
}

t_lxstate	handle_finish(t_lexer *lx)
{
	return (lx->state);
}
