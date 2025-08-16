/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: migugar2 <migugar2@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/03 14:22:22 by migugar2          #+#    #+#             */
/*   Updated: 2025/08/03 18:13:39 by migugar2         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	free_redirs(t_redirs *list)
{
	t_redir	*current;
	t_redir	*next;

	if (!list)
		return ;
	current = list->head;
	while (current)
	{
		next = current->next;
		if (current->u_data.word != NULL)
			free_tok(&current->u_data.word);
		free(current);
		current = next;
	}
	list->head = NULL;
	list->tail = NULL;
}

void	free_ast_cmd_parse(t_ast **ast)
{
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
