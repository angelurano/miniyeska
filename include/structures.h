/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   structures.h                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: migugar2 <migugar2@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/26 02:42:29 by migugar2          #+#    #+#             */
/*   Updated: 2025/09/11 18:45:54 by migugar2         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef STRUCTURES_H
# define STRUCTURES_H

# include <sys/types.h> // pid_t, size_t
# include <stdint.h> // uint8_t
# include <termios.h> // struct termios

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

typedef enum e_segflags
{
	SEGF_NONE = 0,
	SEGF_QUOTED = 1 << 0,
	SEGF_DOUBLE = 1 << 1,
}	t_segflags;

// * segment within a t_tok of type T_WORD, list in order of appearance
typedef struct s_seg
{
	t_segtype		type;
	t_slice			slice;
	uint8_t			flags;
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

// *expansion
/*
 * types of atoms in a builder for expansion:
 * - ATOM_END: end of atoms marker
 * - ATOM_LIT: literal text
 * - ATOM_WILD: wildcard '*'
 */
typedef enum e_atomtype
{
	ATOM_END = 0,
	ATOM_LIT,
	ATOM_WILD
}	t_atomtype;

/*
 * atom node in a builder linked list for expansion
 * represents a piece of a word, either literal text or wildcard
 * and allows to build complex words with multiple segments
 * value points to the original input line and must not be freed
 */
typedef struct s_atom
{
	t_atomtype		type;
	const char		*value;
	size_t			len;
	struct s_atom	*next;
}	t_atom;

// * bitwise flags for t_builder
typedef enum e_buildflags
{
	BUILDF_NONE = 0,
	BUILDF_FINISH = 1 << 0,
	BUILDF_ASSIGN = 1 << 1,
	BUILDF_LEFT = 1 << 2,
	BUILDF_EQ = 1 << 3,
	BUILDF_WILD = 1 << 4,
}	t_buildflags;

/*
 * builder struct for expansion
 * a builder represents a single argument divided in atoms
 * can be expanded into multiple arguments if it contains wildcards
 */
typedef struct s_builder
{
	t_atom				*head;
	t_atom				*tail;
	uint8_t				flags;
	struct s_builder	*next;
}	t_builder;

/*
 * expansion struct for building expanded arguments
 * last_status is itoa version of last exit status for '$?', and must be freed
 */
typedef struct s_expand
{
	t_builder	*head;
	t_builder	*tail;
	char		*last_status_str;
	uint8_t		is_assign;
}	t_expand;

// * list of arguments for a t_tok expansion, for a command or redir word
typedef struct s_argv
{
	t_list	*head;
	t_list	*tail;
	size_t	argc;
}	t_argv;

// * main

/*
 * environment variable node,
 * with full line of the env,
 * the value is the line after '=' of env
 */
typedef struct s_env
{
	char			*full;
	char			*value;
	struct s_env	*next;
}	t_env;

/*
 * list of environment variables
 * envp is a cached value that will be free on modified list
 */
typedef struct s_env_list
{
	t_env		*head;
	t_env		*tail;
	char		**envp;
	size_t		size;
}	t_env_list;

/*
 * main shell struct t_shell
 * line is the readline input
 * tokens was generated for the lexer from the current line
 * ast was generated for the parser from the tokens
 * last_status is the exit status of the last executed line
 */
typedef struct s_shell
{
	char		*line;
	t_tok		*tokens;
	t_ast		*ast;
	t_env_list	env_list;
	int			interactive;
	uint8_t		last_status;
}	t_shell;

#endif
