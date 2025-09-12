/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   operators.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: migugar2 <migugar2@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/26 21:48:15 by migugar2          #+#    #+#             */
/*   Updated: 2025/09/11 18:18:24 by migugar2         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	parse_cmd_subsh(t_shell *shell, t_tok **cur, t_ast **out)
{
	if (*cur && (*cur)->type == T_LPAREN)
		return (parse_subsh(shell, cur, out));
	return (parse_cmd(shell, cur, out));
}

t_ast	*new_op_node(t_asttype type, t_ast *left, t_ast *right)
{
	t_ast	*ast;

	ast = malloc(sizeof(t_ast));
	if (ast == NULL)
		return (NULL);
	ast->type = type;
	ast->u_data.op.left = left;
	ast->u_data.op.right = right;
	return (ast);
}

int	parse_pipe(t_shell *shell, t_tok **cur, t_ast **out)
{
	t_ast	*right;

	if (parse_cmd_subsh(shell, cur, out) == 1)
		return (1);
	while (*cur && (*cur)->type == T_PIPE)
	{
		consume_tok(cur);
		right = NULL;
		if (parse_cmd_subsh(shell, cur, &right) == 1)
			return (free_ast_parse(out), 1);
		*out = new_op_node(AST_PIPE, *out, right);
		if (*out == NULL)
			return (perror_malloc(shell), free_ast_parse(out),
				free_ast_parse(&right), 1);
	}
	return (0);
}

int	parse_and_or(t_shell *shell, t_tok **cur, t_ast **out)
{
	t_asttype	type;
	t_ast		*right;

	if (parse_pipe(shell, cur, out) == 1)
		return (1);
	while (*cur
		&& ((*cur)->type == T_AND_IF || (*cur)->type == T_OR_IF))
	{
		if ((*cur)->type == T_AND_IF)
			type = AST_AND_IF;
		else
			type = AST_OR_IF;
		consume_tok(cur);
		right = NULL;
		if (parse_pipe(shell, cur, &right) == 1)
			return (free_ast_parse(out), 1);
		*out = new_op_node(type, *out, right);
		if (*out == NULL)
			return (perror_malloc(shell), free_ast_parse(out),
				free_ast_parse(&right), 1);
	}
	return (0);
}
