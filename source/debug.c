#include "minishell.h"

#define DEBUG_INDENT_SIZE 2

/*
static const char *segtype_names[] = {};
static const char *toktype_names[] = {};
static const char *redirtype_names[] = {};
static const char *asttype_names[] = {};
*/
// =============================================================================

const char *g_segtype_names[] = { "SEG_TEXT", "SEG_PARAM", "SEG_WILDCARD" };

const char *g_toktype_names[] = { "T_WORD", "T_PIPE", "T_AND_IF", "T_OR_IF", "T_INFILE", "T_HEREDOC", "T_OUTFILE", "T_APPEND", "T_LPAREN", "T_RPAREN" };

const char *g_redirtype_names[] = { "R_INFILE", "R_HEREDOC", "R_OUTFILE", "R_APPEND" };

const char *g_asttype_names[] = { "AST_CMD", "AST_PIPE", "AST_AND_IF", "AST_OR_IF", "AST_SUBSH" };

// =============================================================================

void	debug_indent(int level)
{
	printf("%*s", level * DEBUG_INDENT_SIZE, "");
}

void	debug_print_header(const char *section_name)
{
	printf("\n=== %s ===\n", section_name);
}

void	debug_print_null_message(const char *item_name, int level)
{
	debug_indent(level);
	printf("NULL %s\n", item_name);
}

// =============================================================================

void	debug_print_segment_info(t_seg *seg)
{
	printf("Segment [%s]: '%.*s' (%s)\n",
		g_segtype_names[seg->type],
		(int)seg->slice.len,
		seg->slice.begin,
		seg->flags & SEGF_QUOTED
			? seg->flags & SEGF_DOUBLE
				? "double-quoted"
				: "single-quoted"
			: "unquoted"
	);
}

void	debug_seg(t_seg *seg, int level)
{
	if (!seg)
	{
		debug_print_null_message("segment", level);
		return ;
	}
	debug_indent(level);
	debug_print_segment_info(seg);
}

void	debug_segment_list(t_seg *seg_head, int level)
{
	t_seg	*current;

	current = seg_head;
	while (current)
	{
		debug_seg(current, level);
		current = current->next;
	}
}

// =============================================================================

void	debug_print_token_info(t_tok *tok)
{
	printf("Token [%s]\n", g_toktype_names[tok->type]);
}

void	debug_tok(t_tok *tok, int level)
{
	if (!tok)
	{
		debug_print_null_message("token", level);
		return ;
	}
	debug_indent(level);
	debug_print_token_info(tok);
	debug_segment_list(tok->seg_head, level + 1);
}

void	debug_token_list(t_tok *head, int level)
{
	t_tok	*current;

	current = head;
	while (current)
	{
		debug_tok(current, level);
		current = current->next;
	}
}

// =============================================================================

void	debug_print_redir_info(t_redir *redir)
{
	printf("Redirection [%s]\n", g_redirtype_names[redir->type]);
}

void	debug_redir(t_redir *redir, int level)
{
	if (!redir)
	{
		debug_print_null_message("redirection", level);
		return ;
	}
	debug_indent(level);
	debug_print_redir_info(redir);
}

void	debug_redir_list(t_redir *head, int level)
{
	t_redir	*current;

	current = head;
	while (current)
	{
		debug_redir(current, level);
		current = current->next;
	}
}

void	debug_redirs(t_redirs *redirs, int level)
{
	if (!redirs || !redirs->head)
		return ;
	debug_redir_list(redirs->head, level);
}

// =============================================================================

int	debug_ast_node(t_shell *shell, t_ast **ast, int level);

void	debug_expanded_cmd_words(t_cmd *cmd, int level)
{
	size_t	i;

	if (!cmd || !cmd->u_data.argv)
	{
		debug_indent(level);
		printf("No command words found\n");
		return ;
	}
	debug_indent(level);
	printf("Expanded command words (%zu):\n", cmd->count);
	i = 0;
	while (i < cmd->count)
	{
		debug_indent(level + 1);
		printf("[%zu] '%s'\n", i, cmd->u_data.argv[i]);
		i++;
	}
}

void	debug_expanded_redirs(t_redirs *redirs, int level)
{
	t_redir	*current;

	if (!redirs || !redirs->head)
		return ;
	debug_indent(level);
	printf("Expanded redirections:\n");
	current = redirs->head;
	while (current)
	{
		debug_indent(level + 1);
		printf("[%s] -> '%s'\n",
			g_redirtype_names[current->type],
			current->u_data.name);
		current = current->next;
	}
}

// =============================================================================

int	debug_ast_cmd(t_shell *shell, t_ast **ast, int level)
{
	t_tok	*word;

	if (!ast || (*ast)->type != AST_CMD)
		return (0);

	debug_indent(level);
	printf("Command tokens:\n");
	word = (*ast)->u_data.cmd.u_data.words;
	while (word)
	{
		debug_tok(word, level + 1);
		word = word->next;
	}

	debug_indent(level);
	printf("Original redirections:\n");
	debug_redirs(&(*ast)->u_data.cmd.redir, level + 1);

	if (expand_cmd(shell, &(*ast)->u_data.cmd) == 1)
	{
		return (1);
	}

	debug_expanded_cmd_words(&(*ast)->u_data.cmd, level);
	debug_expanded_redirs(&(*ast)->u_data.cmd.redir, level);
	return (0);
}

int	debug_ast_subsh(t_shell *shell, t_ast **ast, int level)
{
	if (!ast || !*ast || (*ast)->type != AST_SUBSH)
		return (0);

	debug_indent(level);
	printf("Subshell redirections:\n");
	debug_redirs(&(*ast)->u_data.subsh.redir, level + 1);

	if (expand_subsh(shell, &(*ast)->u_data.subsh) == 1)
	{
		return (1);
	}

	debug_expanded_redirs(&(*ast)->u_data.subsh.redir, level);

	debug_indent(level);
	printf("Subshell content:\n");
	if (debug_ast_node(shell, &(*ast)->u_data.subsh.child, level + 1) == 1)
	{
		return (1);
	}
	return (0);
}

// TODO: this is a free example when expanded or fail expanded, if everything works, free_exp_ast over shell->ast works
// TODO: in case of error, free only the part that has been expanded, for everything, deepest than current is expanded, if is operator and fails in right, left is expanded
// TODO: also, check if can free every command or node when is expanded, and used
int	debug_ast_operator(t_shell *shell, t_ast **ast, int level)
{
	if (!ast || !*ast)
		return (0);

	debug_indent(level);
	printf("Left operand:\n");
	if (debug_ast_node(shell, &(*ast)->u_data.op.left, level + 1) == 1)
	{
		return (1);
	}

	debug_indent(level);
	printf("Right operand:\n");
	if (debug_ast_node(shell, &(*ast)->u_data.op.right, level + 1) == 1)
	{
		free_exp_ast(&(*ast)->u_data.op.left);
		return (1);
	}
	return (0);
}

int	debug_ast_node(t_shell *shell, t_ast **ast, int level)
{
	if (!ast || !*ast)
	{
		debug_print_null_message("AST node", level);
		return (0);
	}

	debug_indent(level);
	printf("AST Node [%s]:\n", g_asttype_names[(*ast)->type]);

	if ((*ast)->type == AST_CMD)
		return (debug_ast_cmd(shell, ast, level + 1));
	else if ((*ast)->type == AST_SUBSH)
		return (debug_ast_subsh(shell, ast, level + 1));
	else
		return (debug_ast_operator(shell, ast, level + 1));
}

// =============================================================================

int	debug_tokenizer(t_shell *shell)
{
	debug_print_header("TOKENIZER OUTPUT");
	if (!shell->tokens)
	{
		printf("No tokens found.\n");
		return (0);
	}
	debug_token_list(shell->tokens, 1);
	return (0);
}

int	debug_parser(t_shell *shell)
{
	debug_print_header("PARSER OUTPUT");
	if (!shell->ast)
	{
		printf("No AST generated.\n");
		return (0);
	}
	if (debug_ast_node(shell, &shell->ast, 1) == 1)
		return (1);
	return (0);
}
