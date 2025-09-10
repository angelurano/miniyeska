/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirs.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: migugar2 <migugar2@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/16 10:59:22 by migugar2          #+#    #+#             */
/*   Updated: 2025/08/30 13:11:54 by migugar2         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	expand_redir(t_shell *shell, t_redir *redir)
{
	t_argv	argv;

	argv.head = NULL;
	argv.tail = NULL;
	argv.argc = 0;
	if (expansion(shell, redir->u_data.word, &argv, 0) == 1)
		return (1);
	if (argv.argc == 0 || argv.argc > 1)
		return (perror_ambiguosredir(shell, redir->u_data.word),
			free_argv(&argv), 1);
	free_tok(&redir->u_data.word);
	redir->u_data.name = (char *)argv.head->content;
	ft_lstdelone(argv.head, NULL);
	return (0);
}

int	expand_redirs(t_shell *shell, t_redirs *redirs)
{
	t_redir	*cur;
	t_redir	*bef;

	bef = NULL;
	cur = redirs->head;
	while (cur != NULL)
	{
		if (cur->type != R_HEREDOC)
		{
			if (expand_redir(shell, cur) == 1)
			{
				if (bef != NULL)
					bef->next = NULL;
				else
					redirs->head = NULL;
				return (free_redirslst(&cur),
					free_exp_redirslst(&redirs->head), 1);
			}
		}
		bef = cur;
		cur = cur->next;
	}
	return (0);
}
