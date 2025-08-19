/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free2.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: migugar2 <migugar2@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/17 18:36:27 by migugar2          #+#    #+#             */
/*   Updated: 2025/08/17 19:27:40 by migugar2         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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

void	free_ast_cmd_final(t_ast **ast)
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

void	free_ast_final(t_ast **ast)
{
	if (!ast || !*ast)
		return ;
	if ((*ast)->type == AST_PIPE || (*ast)->type == AST_AND_IF
		|| (*ast)->type == AST_OR_IF)
	{
		free_ast_final(&(*ast)->u_data.op.left);
		free_ast_final(&(*ast)->u_data.op.right);
	}
	else if ((*ast)->type == AST_SUBSH)
	{
		free_ast_final(&(*ast)->u_data.subsh.child);
		free_exp_redirs(&(*ast)->u_data.subsh.redir);
	}
	else if ((*ast)->type == AST_CMD)
		free_ast_cmd_final(ast);
	free(*ast);
	*ast = NULL;
}
