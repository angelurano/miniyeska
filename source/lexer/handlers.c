/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handlers.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: migugar2 <migugar2@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/24 21:09:42 by migugar2          #+#    #+#             */
/*   Updated: 2025/09/04 01:43:51 by migugar2         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	is_param_start(const char *cur)
{
	if (cur == NULL)
		return (0);
	return (*cur == '$' && (cur[1] == '?' || cur[1] == '_'
			|| ft_isalpha(cur[1])));
}

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
	lx->tok->seg_tail->flags |= SEGF_QUOTED;
	lx_advance(lx);
	return (LX_GENERAL);
}

t_lxstate	handle_in_double_q(t_lexer *lx)
{
	const char	*start;

	start = lx->cur;
	while (*lx->cur && *lx->cur != '\"')
	{
		if (is_param_start(lx->cur))
			break ;
		lx_advance(lx);
	}
	if (*lx->cur == '\0')
		return (LX_DIE);
	if (start_word(lx) == 1)
		return (LX_ERR);
	if (add_seg(lx, SEG_TEXT, start, lx->cur - start) == 1)
		return (LX_ERR);
	lx->tok->seg_tail->flags |= SEGF_QUOTED | SEGF_DOUBLE;
	if (is_param_start(lx->cur))
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
	if (lx->prev == LX_IN_DOUBLE_Q)
		lx->tok->seg_tail->flags |= SEGF_QUOTED | SEGF_DOUBLE;
	return (lx->prev);
}

t_lxstate	handle_finish(t_lexer *lx)
{
	return (lx->state);
}
