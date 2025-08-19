/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirs.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: migugar2 <migugar2@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/16 10:59:22 by migugar2          #+#    #+#             */
/*   Updated: 2025/08/17 18:58:01 by migugar2         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	expand_redir(t_shell *shell, t_redir *redir)
{
	t_argv	argv;

	argv.head = NULL;
	argv.tail = NULL;
	argv.argc = 0;
	if (expansion(shell, redir->u_data.word, &argv) == 1)
		return (1);
	if (argv.argc > 1)
		return (perror_ambiguosredir(redir->u_data.word), free_argv(&argv), 1);
	free_tok(&redir->u_data.word);
	redir->u_data.name = (char *)argv.head->content;
	ft_lstdelone(argv.head, NULL);
	return (0);
}

int	expand_redirs(t_shell *shell, t_redirs *redirs)
{
	t_redir	*cur;

	cur = redirs->head;
	while (cur != NULL)
	{
		if (cur->type != R_HEREDOC)
		{
			if (expand_redir(shell, cur) == 1)
				return (free_redirslst(&cur),
					free_exp_redirslst(&redirs->head), 1);
		}
		cur = cur->next;
	}
	return (0);
}
