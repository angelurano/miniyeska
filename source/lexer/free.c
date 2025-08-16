/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: migugar2 <migugar2@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/24 21:41:20 by migugar2          #+#    #+#             */
/*   Updated: 2025/07/24 22:44:19 by migugar2         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	free_segments(t_seg **seg)
{
	t_seg	*next;

	if (!seg || !*seg)
		return ;
	while (*seg)
	{
		next = (*seg)->next;
		free(*seg);
		*seg = next;
	}
}

void	free_tok(t_tok **tok)
{
	if (!tok || !*tok)
		return ;
	free_segments(&(*tok)->seg_head);
	free(*tok);
	*tok = NULL;
}

void	free_tokens(t_tok **tok)
{
	t_tok	*next;

	if (!tok || !*tok)
		return ;
	while (*tok)
	{
		next = (*tok)->next;
		free_tok(tok);
		*tok = next;
	}
}
