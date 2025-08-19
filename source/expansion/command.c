/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: migugar2 <migugar2@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/16 16:52:47 by migugar2          #+#    #+#             */
/*   Updated: 2025/08/16 17:13:17 by migugar2         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	expand_cmd(t_shell *shell, t_cmd *cmd)
{
	t_argv	argv;
	t_tok	*next;

	if (expand_redirs(shell, &cmd->redir) == 1)
		return (1);
	argv.head = NULL;
	argv.tail = NULL;
	argv.argc = 0;
	while (cmd->u_data.words != NULL)
	{
		next = cmd->u_data.words->next;
		cmd->u_data.words->next = NULL;
		if (expansion(shell, cmd->u_data.words, &argv) == 1)
			return (free_argv(&argv), free_exp_redirs(&cmd->redir), 1);
		free_tok(&cmd->u_data.words);
		cmd->u_data.words = next;
	}
	cmd->u_data.argv = NULL;
	cmd->u_data.argv = convert_argv_to_array(&argv);
	if (cmd->u_data.argv == NULL)
		return (free_argv(&argv), free_exp_redirs(&cmd->redir), 1);
	cmd->count = argv.argc;
	return (0);
}

int	expand_subsh(t_shell *shell, t_cmd *cmd)
{
	if (expand_redirs(shell, &cmd->redir) == 1)
		return (1);
	return (0);
}
