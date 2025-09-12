/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: migugar2 <migugar2@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/23 03:41:41 by migugar2          #+#    #+#             */
/*   Updated: 2025/09/11 18:10:33 by migugar2         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	lx_advance(t_lexer *lx)
{
	if (lx->cur && *lx->cur != '\0')
		lx->cur++;
}

void	lx_advance_n(t_lexer *lx, size_t n)
{
	size_t	i;

	i = 0;
	while (i < n && lx->cur && *lx->cur != '\0')
	{
		lx->cur++;
		i++;
	}
}

static t_lxhandler	lexer_handler(t_lxstate state)
{
	if (state == LX_GENERAL)
		return (handle_general);
	else if (state == LX_IN_SINGLE_Q)
		return (handle_in_single_q);
	else if (state == LX_IN_DOUBLE_Q)
		return (handle_in_double_q);
	else if (state == LX_PARAM)
		return (handle_param);
	return (handle_finish);
}

static int	lexer_error(t_shell *shell, t_lexer *lx)
{
	shell->tokens = NULL;
	free_tokens(&lx->head);
	if (lx->state == LX_ERR)
		return (perror_malloc(shell));
	else if (*lx->cur == '\0')
		perror_unexpecteof(shell, lx->prev);
	return (0);
}

int	tokenize(t_shell *shell)
{
	t_lexer		lx;
	t_lxstate	next;

	lx.tok = NULL;
	lx.head = NULL;
	lx.tail = NULL;
	lx.input = shell->line;
	lx.cur = shell->line;
	lx.state = LX_GENERAL;
	while (lx.state != LX_EOL && lx.state != LX_DIE && lx.state != LX_ERR)
	{
		next = lexer_handler(lx.state)(&lx);
		lx.prev = lx.state;
		lx.state = next;
	}
	emit_tok(&lx);
	if (lx.state == LX_ERR || lx.state == LX_DIE)
		return (lexer_error(shell, &lx));
	shell->tokens = lx.head;
	return (0);
}
