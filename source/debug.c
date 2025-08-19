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

// void	debug_tok_expanded(t_tok *tok, int level)
// {
// 	debug_tok(tok, level);
// 	if (tok->type == T_WORD)
// 	{
// 	}
// }

void	debug_redir(t_redir *redir, int level)
{
	debug_indent(level);
	printf("Redir type: %s (%d)\n", redirtype_names[redir->type], redir->type);
}

// void	debug_redir_expanded(t_redir *redir, int level)
// {
// 	debug_redir(redir, level);
// 	debug_indent(level + 1);
// 	if (redir->u_data.word == NULL)
// 		printf("Redir NULL\n");
// 	else
// 	{
// 		char *literal = literal_expansion(redir->u_data.word);
// 		if (redir->type == R_HEREDOC)
// 		{
// 			char *expanded = simple_expansion(redir->u_data.word);
// 			printf("Literal redir: %s, Expanded redir (simple): %s\n", literal, expanded);
// 			free(expanded);
// 		}
// 		else
// 		{
// 		}
// 		free(literal);
// 	}
// }

static void	debug_redirs(t_redirs *redirs, int level)
{
	if (redirs == NULL || redirs->head == NULL)
		return ;
	// debug_indent(level);
	// printf("Redirs:\n");
	t_redir	*redir = redirs->head;
	while (redir != NULL)
	{
		// debug_redir_expanded(redir, level);
		debug_redir(redir, level);
		redir = redir->next;
	}
}

static void	debug_ast(t_shell *shell, t_ast *ast, int level)
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
		t_tok	*word = ast->u_data.cmd.u_data.words;
		while (word != NULL)
		{
			debug_tok(word, level + 1);
			word = word->next;
		}
		debug_redirs(&ast->u_data.cmd.redir, level + 1);
		expand_cmd(shell, &ast->u_data.cmd);
		debug_indent(level + 1);
		printf("cmd expanded:\n");
		size_t i = 0;
		while (i < ast->u_data.cmd.count)
		{
			debug_indent(level + 2);
			printf("Word %zu: %s\n", i, ast->u_data.cmd.u_data.argv[i]);
			i++;
		}
		t_redir	*redir = ast->u_data.cmd.redir.head;

		debug_indent(level + 1);
		printf("Redirs expanded:\n");
		while (redir != NULL)
		{
			debug_indent(level + 2);
			// debug_redir_expanded(redir, level);
			printf("Redir type: %s (%d): [%s]\n", redirtype_names[redir->type], redir->type, redir->u_data.name);
			redir = redir->next;
		}
}
	else if (ast->type == AST_SUBSH)
	{
		debug_ast(shell, ast->u_data.subsh.child, level + 1);
		debug_redirs(&ast->u_data.subsh.redir, level + 1);
	}
	else {
		debug_indent(level + 1);
		printf("Left:\n");
		debug_ast(shell, ast->u_data.op.left, level + 2);
		debug_indent(level + 1);
		printf("Right:\n");
		debug_ast(shell, ast->u_data.op.right, level + 2);
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

void	debug_parser(t_shell *shell, t_ast *ast)
{
	printf("-> Parser:\n");
	debug_ast(shell, ast, 1);
}
