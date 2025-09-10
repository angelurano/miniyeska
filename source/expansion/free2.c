/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free2.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: migugar2 <migugar2@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/17 18:36:27 by migugar2          #+#    #+#             */
/*   Updated: 2025/09/09 20:55:27 by migugar2         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	free_exp_redir(t_redir **redir)
{
	if (redir == NULL || *redir == NULL)
		return ;
	if ((*redir)->type != R_HEREDOC)
	{
		free((*redir)->u_data.name);
		(*redir)->u_data.name = NULL;
	}
	else
	{
		// TODO: remove free_tok and replace with ft_close(&(*redir)->u_data.pipefd[0]);
		free_tok(&(*redir)->u_data.word);
	}
	free(*redir);
	*redir = NULL;
}

void	free_exp_redirslst(t_redir **head)
{
	t_redir	*next;

	if (head == NULL || *head == NULL)
		return ;
	while (*head != NULL)
	{
		next = (*head)->next;
		free_exp_redir(head);
		*head = next;
	}
}

void	free_exp_redirs(t_redirs *redirs)
{
	if (redirs == NULL)
		return ;
	free_exp_redirslst(&redirs->head);
	redirs->tail = NULL;
}

void	free_exp_ast_cmd(t_ast **ast)
{
	size_t	i;

	if (!ast || !*ast)
		return ;
	if ((*ast)->u_data.cmd.u_data.argv != NULL)
	{
		i = 0;
		while (i < (*ast)->u_data.cmd.count)
		{
			free((*ast)->u_data.cmd.u_data.argv[i]);
			(*ast)->u_data.cmd.u_data.argv[i] = NULL;
			i++;
		}
		free((*ast)->u_data.cmd.u_data.argv);
		(*ast)->u_data.cmd.u_data.argv = NULL;
	}
	free_exp_redirs(&(*ast)->u_data.cmd.redir);
	free(*ast);
	*ast = NULL;
}

// TODO: Analyzing, this probably must don't work every time, because expansion probably will be in a forked process, not in the main process
void	free_exp_ast(t_ast **ast)
{
	if (!ast || !*ast)
		return ;
	if ((*ast)->type == AST_PIPE || (*ast)->type == AST_AND_IF
		|| (*ast)->type == AST_OR_IF)
	{
		free_exp_ast(&(*ast)->u_data.op.left);
		free_exp_ast(&(*ast)->u_data.op.right);
	}
	else if ((*ast)->type == AST_SUBSH)
	{
		free_exp_ast(&(*ast)->u_data.subsh.child);
		free_exp_redirs(&(*ast)->u_data.subsh.redir);
	}
	else if ((*ast)->type == AST_CMD)
		free_exp_ast_cmd(ast);
	free(*ast);
	*ast = NULL;
}
