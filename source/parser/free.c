/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: migugar2 <migugar2@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/03 14:22:22 by migugar2          #+#    #+#             */
/*   Updated: 2025/08/26 04:01:39 by migugar2         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	free_redirslst(t_redir **head)
{
	t_redir	*next;

	if (!head || !*head)
		return ;
	while (*head != NULL)
	{
		next = (*head)->next;
		if ((*head)->u_data.word != NULL)
			free_tok(&(*head)->u_data.word);
		free(*head);
		*head = next;
	}
	*head = NULL;
}

void	free_redirs(t_redirs *redirs)
{
	if (!redirs)
		return ;
	free_redirslst(&redirs->head);
	redirs->head = NULL;
	redirs->tail = NULL;
}

void	free_ast_cmd_parse(t_ast **ast)
{
	if (!ast || !*ast)
		return ;
	if ((*ast)->u_data.cmd.u_data.words != NULL)
		free_tokens(&(*ast)->u_data.cmd.u_data.words);
	free_redirs(&(*ast)->u_data.cmd.redir);
	free(*ast);
	*ast = NULL;
}

void	free_ast_parse(t_ast **ast)
{
	if (!ast || !*ast)
		return ;
	if ((*ast)->type == AST_PIPE || (*ast)->type == AST_AND_IF
		|| (*ast)->type == AST_OR_IF)
	{
		free_ast_parse(&(*ast)->u_data.op.left);
		free_ast_parse(&(*ast)->u_data.op.right);
	}
	else if ((*ast)->type == AST_SUBSH)
	{
		free_ast_parse(&(*ast)->u_data.subsh.child);
		free_redirs(&(*ast)->u_data.subsh.redir);
	}
	else if ((*ast)->type == AST_CMD)
		free_ast_cmd_parse(ast);
	free(*ast);
	*ast = NULL;
}
