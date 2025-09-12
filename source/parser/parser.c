/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: migugar2 <migugar2@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/25 02:07:43 by migugar2          #+#    #+#             */
/*   Updated: 2025/09/11 18:22:53 by migugar2         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	is_redirtok(t_tok *tok)
{
	if (tok == NULL)
		return (0);
	return (tok->type == T_INFILE || tok->type == T_OUTFILE
		|| tok->type == T_HEREDOC || tok->type == T_APPEND);
}

void	consume_tok(t_tok **cur)
{
	t_tok	*next;

	if (*cur == NULL)
		return ;
	next = (*cur)->next;
	free_tok(cur);
	*cur = next;
}

char	*get_text_tok(t_tok *tok)
{
	if (tok->type == T_AND_IF)
		return ("&&");
	else if (tok->type == T_OR_IF)
		return ("||");
	else if (tok->type == T_PIPE)
		return ("|");
	else if (tok->type == T_LPAREN)
		return ("(");
	else if (tok->type == T_RPAREN)
		return (")");
	else if (tok->type == T_INFILE)
		return ("<");
	else if (tok->type == T_OUTFILE)
		return (">");
	else if (tok->type == T_HEREDOC)
		return ("<<");
	else if (tok->type == T_APPEND)
		return (">>");
	return ("");
}

int	parse_ast(t_shell *shell)
{
	t_tok	*cur;

	cur = shell->tokens;
	shell->ast = NULL;
	if (parse_and_or(shell, &cur, &shell->ast) == 1)
		return (free_tokens(&cur), free_ast_parse(&shell->ast), 1);
	if (cur != NULL)
		return (perror_syntaxtok(shell, cur), free_tokens(&cur),
			free_ast_parse(&shell->ast), 1);
	return (0);
}
