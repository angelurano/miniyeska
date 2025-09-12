/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: migugar2 <migugar2@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/26 21:46:00 by migugar2          #+#    #+#             */
/*   Updated: 2025/09/12 20:27:48 by migugar2         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_ast	*new_cmd_leaf(void)
{
	t_ast	*ast;

	ast = malloc(sizeof(t_ast));
	if (ast == NULL)
		return (NULL);
	ast->type = AST_CMD;
	ast->u_data.cmd.u_data.words = NULL;
	ast->u_data.cmd.count = 0;
	ast->u_data.cmd.redir.head = NULL;
	ast->u_data.cmd.redir.tail = NULL;
	return (ast);
}

void	collect_cmd(t_tok **cur, t_ast *cmd, t_tok **last_word)
{
	t_tok	*tok;

	tok = *cur;
	*cur = (*cur)->next;
	tok->next = NULL;
	if (cmd->u_data.cmd.u_data.words == NULL)
		cmd->u_data.cmd.u_data.words = tok;
	else
		(*last_word)->next = tok;
	*last_word = tok;
	cmd->u_data.cmd.count++;
}

int	append_redir(t_shell *shell, t_tok **cur, t_tok **rdirs, t_tok **rdirs_last)
{
	if (*rdirs == NULL)
		*rdirs = *cur;
	else
		(*rdirs_last)->next = *cur;
	*rdirs_last = *cur;
	*cur = (*cur)->next;
	if ((*cur) == NULL)
		return (perror_unexpecteol(shell), 1);
	*rdirs_last = *cur;
	*cur = (*cur)->next;
	(*rdirs_last)->next = NULL;
	if ((*rdirs_last)->type != T_WORD)
		return (perror_syntaxtok(shell, *rdirs_last), 1);
	return (0);
}

int	parse_cmd(t_shell *shell, t_tok **cur, t_ast **out)
{
	t_ast	*cmd;
	t_tok	*last_word;
	t_tok	*redirs_tok;
	t_tok	*redirs_last_tok;

	if (*cur == NULL)
		return (perror_unexpecteol(shell));
	if ((*cur)->type != T_WORD && !is_redirtok(*cur))
		return (perror_syntaxtok(shell, *cur));
	cmd = new_cmd_leaf();
	if (cmd == NULL)
		return (perror_malloc(shell));
	last_word = NULL;
	redirs_tok = NULL;
	while (*cur && ((*cur)->type == T_WORD || is_redirtok(*cur)))
	{
		if ((*cur)->type == T_WORD)
			collect_cmd(cur, cmd, &last_word);
		else if (append_redir(shell, cur, &redirs_tok, &redirs_last_tok) == 1)
			return (free_ast_parse(&cmd), free_tokens(&redirs_tok), 1);
	}
	if (collect_redirs(shell, &redirs_tok, &cmd->u_data.cmd.redir) == 1)
		return (free_ast_parse(&cmd), 1);
	return (*out = cmd, 0);
}

t_ast	*new_subsh_node(t_ast *child)
{
	t_ast	*ast;

	ast = malloc(sizeof(t_ast));
	if (ast == NULL)
		return (NULL);
	ast->type = AST_SUBSH;
	ast->u_data.subsh.child = child;
	ast->u_data.subsh.redir.head = NULL;
	ast->u_data.subsh.redir.tail = NULL;
	return (ast);
}

int	parse_subsh(t_shell *shell, t_tok **cur, t_ast **out)
{
	t_ast	*child;

	consume_tok(cur);
	if (parse_and_or(shell, cur, &child) == 1)
		return (1);
	if (*cur == NULL)
		return (perror_unexpecteol(shell), free_ast_parse(&child), 1);
	if ((*cur)->type != T_RPAREN)
		return (perror_syntaxtok(shell, *cur), free_ast_parse(&child), 1);
	consume_tok(cur);
	child = new_subsh_node(child);
	if (child == NULL)
		return (perror_malloc(shell), free_ast_parse(&child), 1);
	if (collect_redirs(shell, cur, &child->u_data.subsh.redir) == 1)
		return (free_ast_parse(&child), 1);
	*out = child;
	return (0);
}
