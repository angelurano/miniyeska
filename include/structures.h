/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   structures.h                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: migugar2 <migugar2@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/26 02:42:29 by migugar2          #+#    #+#             */
/*   Updated: 2025/08/10 16:03:04 by migugar2         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef STRUCTURES_H
# define STRUCTURES_H

# include <sys/types.h> // pid_t, size_t

// * slice from the original readline buffer (the input line)
typedef struct s_slice
{
	const char	*begin;
	size_t		len;
}	t_slice;

/*
 * types of t_seg :
 * - SEG_TEXT: regular literal text
 * - SEG_PARAM: parameter without '$'
 * - SEG_WILDCARD: wildcard character '*', out of quotes
 */
typedef enum e_segtype
{
	SEG_TEXT,
	SEG_PARAM,
	SEG_WILDCARD
}	t_segtype;

// * segment within a t_tok of type T_WORD, list in order of appearance
typedef struct s_seg
{
	t_segtype		type;
	t_slice			slice;
	struct s_seg	*next;
}	t_seg;

/*
 * types of t_tok:
 * - T_WORD: word token with segments
 * - T_PIPE: operator pipe '|'
 * - T_AND_IF: operator and '&&'
 * - T_OR_IF: operator or '||'
 * - T_INFILE: input redirection '<'
 * - T_HEREDOC: heredoc delimiter '<<'
 * - T_OUTFILE: output redirection '>'
 * - T_APPEND: output redirection '>>'
 * - T_LPAREN: left parenthesis '('
 * - T_RPAREN: right parenthesis ')'
 */
typedef enum e_toktype
{
	T_WORD,
	T_PIPE,
	T_AND_IF,
	T_OR_IF,
	T_INFILE,
	T_HEREDOC,
	T_OUTFILE,
	T_APPEND,
	T_LPAREN,
	T_RPAREN,
}	t_toktype;

/*
 * token's node for linked list, produced by the lexer,
 * T_WORD has segments, others must have NULL seg_head/seg_tail
 */
typedef struct s_tok
{
	t_toktype		type;
	t_seg			*seg_head;
	t_seg			*seg_tail;
	struct s_tok	*next;
}	t_tok;

/*
 * types of redirections t_redir:
 * - R_INFILE: input redirection '<'
 * - R_HEREDOC: heredoc delimiter '<<'
 * - R_OUTFILE: output redirection '>'
 * - R_APPEND: output redirection '>>'
 */
typedef enum e_redirtype
{
	R_INFILE,
	R_HEREDOC,
	R_OUTFILE,
	R_APPEND,
}	t_redirtype;

/*
 * redirection node for redirs linked list, asociated to a cmd or a subsh
 * - u_data union for R_INFILE, R_OUTFILE, R_APPEND:
 *   in parsing is t_tok *word (pre-expansion)
 *   in execution is char *name (post-expansion), is the file name
 * - u_data union for R_HEREDOC:
 *   in parsing is t_tok *word, interpreted literal as delimiter
 *   in execution is int pipefd[2], saves the input user, read from pipefd[0]
 */
typedef struct s_redir
{
	union
	{
		t_tok			*word;
		char			*name;
		int				pipefd[2]; // TODO: For heredoc
	}			u_data;
	t_redirtype		type;
	struct s_redir	*next;
}	t_redir;

// * list of redirections, associated to a cmd or a subsh
typedef struct s_redirs
{
	t_redir		*head;
	t_redir		*tail;
}	t_redirs;

/*
 * AST node types
 * - AST_CMD: leaf simple command: t_cmd (argv/words + redir)
 * - AST_PIPE: pipe binary node: t_op (left | right)
 * - AST_AND_IF: and binary node: t_op (left && right)
 * - AST_OR_IF: or binary node: t_op (left || right)
 * - AST_SUBSH: unary sub-shell node: t_subsh (child), with own optional redirs
 */
typedef enum e_asttype
{
	AST_CMD,
	AST_PIPE,
	AST_AND_IF,
	AST_OR_IF,
	AST_SUBSH,
}	t_asttype;

/*
 * command leaf node in the AST
 * represents a single command with its arguments and redirections
 * - u_data union for command arguments:
 *   in parsing is t_tok *word (pre-expansion)
 *   in execution is char **argv (post-expansion) NULL ended, argv[0] is the cmd
 * - count:
 *   in parsing is the number of words (pre-expansion)
 *   in execution is the argc (post-expansion)
 */
typedef struct s_cmd
{
	union
	{
		t_tok			*words;
		char			**argv;
	}			u_data;
	size_t			count;
	t_redirs		redir;
}	t_cmd;

/*
 * binary operator node in the AST, connect left and right children
 * represents:
 * - priority binary nodes: AST_AND_IF, AST_OR_IF
 * - non-priority binary nodes: AST_PIPE
 */
typedef struct s_op
{
	struct s_ast	*left;
	struct s_ast	*right;
}	t_op;

/*
 * sub-shell node in the AST, represents an sub tree
 * enclosed in parentheses, must be executed in a child process
 * the child is the root of the sub-shell AST
 * redir is the optional list of redirections associated with the sub-shell
 */
typedef struct s_subsh
{
	struct s_ast	*child;
	t_redirs		redir;
}	t_subsh;

/*
 * abstract syntax tree (AST) node
 * depending of type it can represent a cmd, an operator, or a subshell in u_data
 */
typedef struct s_ast
{
	union
	{
		t_cmd	cmd;
		t_op	op;
		t_subsh	subsh;
	}			u_data;
	t_asttype		type;
}	t_ast;

// * lexer

/*
 * lexer states:
 * - LX_GENERAL: general state
 * - LX_IN_SINGLE_Q: inside single quotes
 * - LX_IN_DOUBLE_Q: inside double quotes
 * - LX_PARAM: parameter name
 * - LX_EOL: end of line: valid final state
 * - LX_DIE: invalid state (non closed quotes)
 * - LX_ERR: malloc error state
 */
typedef enum e_lxstate
{
	LX_GENERAL,
	LX_IN_SINGLE_Q,
	LX_IN_DOUBLE_Q,
	LX_PARAM,
	LX_EOL,
	LX_DIE,
	LX_ERR,
}	t_lxstate;

/*
 * lexer struct
 * holds the machine states (prev and state),
 * the processing current char (cur),
 * processing token (tok) and final token list (head)
 */
typedef struct s_lexer
{
	const char		*input;
	const char		*cur;
	t_lxstate		prev;
	t_lxstate		state;
	t_tok			*tok;
	t_tok			*head;
	t_tok			*tail;
}	t_lexer;

// * type for lexer state machine handler function
typedef t_lxstate	(*t_lxhandler)(t_lexer *lx);

#endif
