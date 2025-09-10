/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   general.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: migugar2 <migugar2@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/24 21:09:56 by migugar2          #+#    #+#             */
/*   Updated: 2025/09/04 01:39:24 by migugar2         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static t_lxstate	handle_general_word(t_lexer *lx)
{
	const char	*start;

	start = lx->cur;
	if (start_word(lx) == 1)
		return (LX_ERR);
	while (*lx->cur)
	{
		if (ft_isspace(*lx->cur)
			|| *lx->cur == '|' || *lx->cur == '<' || *lx->cur == '>'
			|| *lx->cur == '*' || *lx->cur == '(' || *lx->cur == ')'
			|| *lx->cur == '\'' || *lx->cur == '"'
			|| (*lx->cur == '&' && lx->cur[1] == '&')
			|| (is_param_start(lx->cur)))
			break ;
		lx_advance(lx);
	}
	if (add_seg(lx, SEG_TEXT, start, lx->cur - start) == 1)
		return (LX_ERR);
	return (LX_GENERAL);
}

static t_lxstate	handle_general_2(t_lexer *lx)
{
	if (*lx->cur == ')')
		return (emit_op(lx, T_RPAREN, 1));
	if (*lx->cur == '\'')
		return (lx_advance(lx), LX_IN_SINGLE_Q);
	if (*lx->cur == '"')
		return (lx_advance(lx), LX_IN_DOUBLE_Q);
	if (is_param_start(lx->cur))
	{
		if (start_word(lx) == 1)
			return (LX_ERR);
		lx_advance(lx);
		return (LX_PARAM);
	}
	if (*lx->cur == '*')
	{
		if (start_word(lx) == 1)
			return (LX_ERR);
		if (add_seg(lx, SEG_WILDCARD, lx->cur, 1) == 1)
			return (LX_ERR);
		lx_advance(lx);
		return (LX_GENERAL);
	}
	return (handle_general_word(lx));
}

t_lxstate	handle_general(t_lexer *lx)
{
	if (ft_isspace(*lx->cur))
	{
		emit_tok(lx);
		while (ft_isspace(*lx->cur))
			lx_advance(lx);
	}
	if (*lx->cur == '\0')
		return (LX_EOL);
	if (*lx->cur == '|' && lx->cur[1] == '|')
		return (emit_op(lx, T_OR_IF, 2));
	if (*lx->cur == '&' && lx->cur[1] == '&')
		return (emit_op(lx, T_AND_IF, 2));
	if (*lx->cur == '<' && lx->cur[1] == '<')
		return (emit_op(lx, T_HEREDOC, 2));
	if (*lx->cur == '>' && lx->cur[1] == '>')
		return (emit_op(lx, T_APPEND, 2));
	if (*lx->cur == '|')
		return (emit_op(lx, T_PIPE, 1));
	if (*lx->cur == '<')
		return (emit_op(lx, T_INFILE, 1));
	if (*lx->cur == '>')
		return (emit_op(lx, T_OUTFILE, 1));
	if (*lx->cur == '(')
		return (emit_op(lx, T_LPAREN, 1));
	return (handle_general_2(lx));
}
