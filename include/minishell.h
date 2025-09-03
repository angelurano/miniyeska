/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: migugar2 <migugar2@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/20 00:02:21 by migugar2          #+#    #+#             */
/*   Updated: 2025/09/01 23:38:31 by migugar2         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "libft.h"

# include "structures.h"

# include <readline/readline.h>
// readline, rl_on_new_line, rl_replace_line, rl_redisplay
# include <readline/history.h>
// add_history, rl_clear_history
# include <stdio.h> // printf, perror
# include <stdlib.h> // exit, malloc, free, getenv
# include <unistd.h>
// write, read, open, close, fork, access, chdir, getcwd, execve
// dup, dup2, pipe, unlink, isatty, ttyname, ttyslot
# include <fcntl.h> // open
# include <sys/types.h> // pid_t
# include <sys/stat.h> // stat, lstat, fstat
# include <sys/wait.h> // wait, waitpid, wait3, wait4, WIFEXITED, WEXITSTATUS
# include <signal.h> // signal, sigaction, kill, SIGINT, SIGQUIT
# include <dirent.h> // opendir, readdir, closedir
# include <errno.h> // errno, strerror
# include <sys/ioctl.h> // ioctl
# include <termios.h> // tcgetattr, tcsetattr
# include <termcap.h>
// tgetent, tgetflag, tgetnum, tgetstr, tgoto, tputs

# define MINI_PROMPT "MINI> $ " // TODO

// extern volatile sig_atomic_t	g_signum; // TODO

int			perror_malloc(void);
int			perror_unexpecteof(t_lxstate prev);
int			perror_unexpecteol(void);
int			perror_syntaxtok(t_tok *cur);
int			perror_ambiguosredir(t_shell *shell, t_tok *word);

// lexer

t_seg		*new_seg(t_segtype k, const char *ptr, size_t len);
int			add_seg(t_lexer *lx, t_segtype type, const char *ptr, size_t len);

t_tok		*new_tok(t_toktype type);
int			start_word(t_lexer *lx);
void		tok_push(t_tok **head, t_tok **tail, t_tok *tok);
void		emit_tok(t_lexer *lx);
t_lxstate	emit_op(t_lexer *lx, t_toktype type, size_t len);

void		lx_advance(t_lexer *lx);
void		lx_advance_n(t_lexer *lx, size_t n);

t_lxstate	handle_general(t_lexer *lx);

t_lxstate	handle_in_single_q(t_lexer *lx);
t_lxstate	handle_in_double_q(t_lexer *lx);
t_lxstate	handle_param(t_lexer *lx);
t_lxstate	handle_finish(t_lexer *lx);

void		free_segments(t_seg **seg);
void		free_tok(t_tok **tok);
void		free_tokens(t_tok **tok);

int			tokenize(const char *input, t_tok **out);

// parser

int			is_redirtok(t_tok *tok);
void		consume_tok(t_tok **cur);
char		*get_text_tok(t_tok *tok);

t_redir		*new_redir(t_redirtype type, t_tok *word);
t_redir		*new_redir_from_tok(t_tok *op, t_tok *word);
void		redir_push(t_redirs *list, t_redir *redir);
int			collect_redir(t_tok **cur, t_redirs *list);

int			collect_redirs(t_tok **cur, t_redirs *redirs);

t_ast		*new_cmd_leaf(void);
void		collect_cmd(t_tok **cur, t_ast *cmd, t_tok **last_word);
int			parse_cmd(t_tok **cur, t_ast **out);
t_ast		*new_subsh_node(t_ast *child);
int			parse_subsh(t_tok **cur, t_ast **out);

int			parse_cmd_subsh(t_tok **cur, t_ast **out);

t_ast		*new_op_node(t_asttype type, t_ast *left, t_ast *right);
int			parse_pipe(t_tok **cur, t_ast **out);
int			parse_and_or(t_tok **cur, t_ast **out);

void		free_redirslst(t_redir **head);
void		free_redirs(t_redirs *list);
void		free_ast_cmd_parse(t_ast **ast);
void		free_ast_parse(t_ast **ast);

int			parse_ast(t_tok *tokens, t_ast **out);

// expansion

int			build_literals(t_shell *shell, t_builder *builder, t_argv *argv);

// t_param		*new_param(void);
// void		param_push(t_param **head, t_param **tail, t_param *new_param);
int			new_argv_push(t_argv *argv, char *value);
int			new_argvdup_push(t_argv *argv, char *value);
char		**convert_argv_to_array(t_argv *argv);

int			expand_redir(t_shell *shell, t_redir *redir);
int			expand_redirs(t_shell *shell, t_redirs *redirs);

// size_t		consume_param(t_exp *exp, t_seg *seg, char *cur);
// t_param		*expand_param(t_shell *shell, t_seg *seg);

// int			wildcard_match(const char *pattern, const char *str);
int			expand_wildcards(t_shell *shell, t_builder *builder, t_argv *argv);

// void		free_param(t_param **param, t_seg *from_seg);
// void		free_paramlst(t_param **head, t_param **tail, t_seg *segs);
void		free_argv(t_argv *argv);
void		free_exp_redir(t_redir **redir);
void		free_exp_redirslst(t_redir **head);
void		free_exp_redirs(t_redirs *redirs);
void		free_ast_cmd_final(t_ast **ast);
void		free_ast_final(t_ast **ast);

// char		*expand_tok(t_exp *exp, t_tok *tok);
int			expand_cmd(t_shell *shell, t_cmd *cmd);
int			expand_subsh(t_shell *shell, t_subsh *subsh);
int			expansion(t_shell *shell, t_tok *tok, t_argv *argv, int is_assign);

char		*literal_expansion(t_tok *word);
char		*simple_expansion(t_tok *word);

// env functions

t_env		*create_env_node(char *full, char *value);
char		*get_env_value(t_env_list *env_list, const char *key);
void		env_list_push(t_env_list *env_list, t_env *node);
void		free_env_list(t_env_list *env_list);

// main helper functions
int			init_shell(t_shell *shell, char *envp[]);

// !debug: delete file and functions
void		debug_tokenizer(t_shell *shell);
int			debug_parser(t_shell *shell);

#endif
