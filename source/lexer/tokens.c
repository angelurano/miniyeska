/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokens.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: migugar2 <migugar2@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/24 21:01:09 by migugar2          #+#    #+#             */
/*   Updated: 2025/07/26 02:06:30 by migugar2         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_tok	*new_tok(t_toktype type)
{
	t_tok	*tok;

	tok = malloc(sizeof(t_tok));
	if (!tok)
		return (NULL);
	tok->type = type;
	tok->seg_head = NULL;
	tok->seg_tail = NULL;
	tok->next = NULL;
	return (tok);
}

int	start_word(t_lexer *lx)
{
	if (lx->tok && lx->tok->type == T_WORD)
		return (0);
	lx->tok = new_tok(T_WORD);
	if (!lx->tok)
		return (1);
	return (0);
}

void	tok_push(t_tok **head, t_tok **tail, t_tok *tok)
{
	if (!tok)
		return ;
	if (*head == NULL)
		*head = tok;
	else
		(*tail)->next = tok;
	*tail = tok;
}

void	emit_tok(t_lexer *lx)
{
	if (!lx->tok)
		return ;
	tok_push(&lx->head, &lx->tail, lx->tok);
	lx->tok = NULL;
}

t_lxstate	emit_op(t_lexer *lx, t_toktype type, size_t len)
{
	t_tok	*operator;

	emit_tok(lx);
	operator = new_tok(type);
	if (!operator)
		return (LX_ERR);
	tok_push(&lx->head, &lx->tail, operator);
	lx_advance_n(lx, len);
	return (LX_GENERAL);
}
