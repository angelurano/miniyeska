/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: migugar2 <migugar2@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/16 16:52:47 by migugar2          #+#    #+#             */
/*   Updated: 2025/09/03 20:20:15 by migugar2         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	expand_cmd_words(t_shell *shell, t_cmd *cmd, t_argv *argv)
{
	int		is_assign;
	int		first_expand;
	t_tok	*next;

	is_assign = 0;
	first_expand = 1;
	while (cmd->u_data.words != NULL)
	{
		next = cmd->u_data.words->next;
		cmd->u_data.words->next = NULL;
		if (expansion(shell, cmd->u_data.words, argv, is_assign) == 1)
			return (free_argv(argv), free_exp_redirs(&cmd->redir), 1);
		free_tok(&cmd->u_data.words);
		if (first_expand == 1)
		{
			if (argv->argc > 0 && ft_strcmp(argv->head->content, "export") == 0)
				is_assign = 1;
			first_expand = 0;
		}
		cmd->u_data.words = next;
	}
	return (0);
}

int	expand_cmd(t_shell *shell, t_cmd *cmd)
{
	t_argv	argv;

	argv.head = NULL;
	argv.tail = NULL;
	argv.argc = 0;
	if (expand_redirs(shell, &cmd->redir) == 1)
		return (1);
	if (expand_cmd_words(shell, cmd, &argv) == 1)
		return (1);
	cmd->u_data.argv = NULL;
	cmd->u_data.argv = convert_argv_to_array(&argv);
	if (cmd->u_data.argv == NULL)
		return (perror_malloc(), free_argv(&argv),
			free_exp_redirs(&cmd->redir), 1);
	cmd->count = argv.argc;
	return (0);
}

int	expand_subsh(t_shell *shell, t_subsh *subsh)
{
	if (expand_redirs(shell, &subsh->redir) == 1)
		return (1);
	return (0);
}
