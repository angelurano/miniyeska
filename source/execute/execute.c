/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gomandam <gomandam@student.42madrid>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/22 00:24:27 by gomandam          #+#    #+#             */
/*   Updated: 2025/08/29 22:38:49 by gomandam         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/*
 * Recursive function that executes the AST node
 * Deals with the logic based on node type:
 * - AST_CMD:     Executes a command
 * - AST_PIPE:    Handles a pipeline (left | right)
 * - AST_AND_IF:  Handles '&&' logic (executes right only if left succeeds)
 * - AST_OR_IF:   Handles '||' logic (executes right only if left fails)
 * - AST_SUBSH:   Handles a subshell (parentheses)
 	* Returns: exit status of the executed subtree·		*/

#include "minishell.h"

int	execute_ast(t_shell *shell, t_ast *node)
{
	int	storage;

	storage = 0;
	if (!node)
		return (0);
	if (node->type == AST_CMD)
		return (exec_ast_cmd(shell, &node->u_data.cmd));
	else if (node->type == AST_PIPE)
		return (exec_ast_pipe(shell, node));
	else if (node->type == AST_AND_IF)
	{
		storage = execute_ast(shell, node->u_data.op.left);
		if (storage == 0)
			return (execute_ast(shell, node->u_data.op.right));
		return (storage);
	}
	else if (node->type == AST_OR_IF)
	{
		storage = execute_ast(shell, node->u_data.op.left);
		if (storage != 0)
			return (execute_ast(shell, node->u_data.op.right));
		return (storage);
	}
	else if (node->type == AST_SUBSH)
	{
		printf("implement subshell logic.");
		return (0);
		// TO DO: implement subshell logic
		// if (expand_subsh(shell, &node->u_data.subsh) != 0)
		//	return (1);
		// subshell execution (fork, exec in child process AST, handle redirs)
	}
	ft_putstr_fd("minishell: internal error: unexpected AST node type \n", 2);
	return (1);
}
