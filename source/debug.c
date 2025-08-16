// ! THIS FILE IS TEMPORAL AND WILL BE DELETED IN THE FUTURE !
#include "minishell.h"

static const char *segtype_names[] = { "SEG_TEXT", "SEG_PARAM", "SEG_WILDCARD" };
static const char *toktype_names[] = { "T_WORD", "T_PIPE", "T_AND_IF", "T_OR_IF", "T_INFILE", "T_HEREDOC", "T_OUTFILE", "T_APPEND", "T_LPAREN", "T_RPAREN" };
static const char *redirtype_names[] = { "R_INFILE", "R_HEREDOC", "R_OUTFILE", "R_APPEND" };
static const char *asttype_names[] = { "AST_CMD", "AST_PIPE", "AST_AND_IF", "AST_OR_IF", "AST_SUBSH" };

static void	debug_indent(int level)
{
	printf("%*s", level * 2, "");
}

static void	debug_seg(t_seg *seg, int level)
{
	debug_indent(level);
	printf("Segment type: %s (%d) [%.*s]\n", segtype_names[seg->type], seg->type, (int)seg->slice.len, seg->slice.begin);
}

void	debug_tok(t_tok *tok, int level)
{
	debug_indent(level);
	printf("Token type: %s (%d)\n", toktype_names[tok->type], tok->type);
	t_seg *seg = tok->seg_head;
	while (seg != NULL)
	{
		debug_seg(seg, level + 1);
		seg = seg->next;
	}
}

static void	debug_redirs(t_redirs *redirs, int level)
{
	if (redirs == NULL || redirs->head == NULL)
		return ;
	// debug_indent(level);
	// printf("Redirs:\n");
	t_redir	*redir = redirs->head;
	while (redir != NULL)
	{
		debug_indent(level);
		printf("Redir type: %s (%d)\n", redirtype_names[redir->type], redir->type);
		if (redir->u_data.word != NULL)
			debug_tok(redir->u_data.word, level + 1);
		else
		{
			debug_indent(level + 1);
			printf("Redir NULL\n");
		}
		redir = redir->next;
	}
}

static void	debug_ast(t_ast *ast, int level)
{
	if (ast == NULL)
	{
		debug_indent(level);
		printf("NULL\n");
		return ;
	}
	debug_indent(level);
	printf("AST type: %s (%d)\n", asttype_names[ast->type], ast->type);
	if (ast->type == AST_CMD)
	{
		// debug_indent(level + 1);
		// printf("Command node:\n");
		t_tok	*word = ast->u_data.cmd.u_data.words;
		while (word != NULL)
		{
			debug_tok(word, level + 1);
			word = word->next;
		}
		debug_redirs(&ast->u_data.cmd.redir, level + 1);
	}
	else if (ast->type == AST_SUBSH)
	{
		// debug_indent(level + 1);
		// printf("Subshell node:\n");
		debug_ast(ast->u_data.subsh.child, level + 1);
		debug_redirs(&ast->u_data.subsh.redir, level + 1);
	}
	else {
		debug_indent(level + 1);
		printf("Left:\n");
		debug_ast(ast->u_data.op.left, level + 2);
		debug_indent(level + 1);
		printf("Right:\n");
		debug_ast(ast->u_data.op.right, level + 2);
	}
}

void	debug_tokenizer(t_tok *head)
{
	printf("-> Tokenizer:\n");
	t_tok	*tok = head;
	while (tok != NULL)
	{
		debug_tok(tok, 1);
		tok = tok->next;
	}
}

void	debug_parser(t_ast *ast)
{
	printf("-> Parser:\n");
	debug_ast(ast, 1);
}
