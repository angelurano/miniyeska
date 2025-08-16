/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   errors.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: migugar2 <migugar2@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/07 15:28:33 by migugar2          #+#    #+#             */
/*   Updated: 2025/08/08 20:03:58 by migugar2         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// TODO: must modify an struct or value for indicate the error code exit, for example 258 for syntax error
int	perror_malloc(void)
{
	write(STDERR_FILENO, "minishell: memory allocation error\n", 35);
	return (1);
}

int	perror_unexpecteof(t_lxstate prev)
{
	if (prev == LX_IN_SINGLE_Q)
		write(STDERR_FILENO,
			"minishell: unexpected EOF while looking for matching `''\n", 58);
	else if (prev == LX_IN_DOUBLE_Q)
		write(STDERR_FILENO,
			"minishell: unexpected EOF while looking for matching `\"'\n", 58);
	return (1);
}

int	perror_unexpecteol(void)
{
	write(STDERR_FILENO,
		"minishell: syntax error near unexpected token `newline'\n", 56);
	return (1);
}

int	perror_syntaxtok(t_tok *cur)
{
	char	*tok_text;
	size_t	tok_size;

	if (cur == NULL)
		write(STDERR_FILENO, "minishell: syntax error unexpected token\n", 41);
	else
	{
		tok_text = get_text_tok(cur);
		tok_size = ft_strlen(tok_text);
		write(STDERR_FILENO, "minishell: syntax error near unexpected token `",
			47);
		write(STDERR_FILENO, tok_text, tok_size);
		write(STDERR_FILENO, "'\n", 2);
	}
	return (1);
}
