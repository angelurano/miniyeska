/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: migugar2 <migugar2@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/20 00:02:21 by migugar2          #+#    #+#             */
/*   Updated: 2025/09/11 22:33:32 by migugar2         ###   ########.fr       */
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

extern volatile sig_atomic_t	g_signum;

int			perror_malloc(t_shell *shell);
int			perror_unexpecteof(t_shell *shell, t_lxstate prev);
int			perror_unexpecteol(t_shell *shell);
int			perror_syntaxtok(t_shell *shell, t_tok *cur);
int			perror_ambiguosredir(t_shell *shell, t_tok *word);
int			perror_cmdnotfound(t_shell *shell, const char *cmd);
int			perror_usage(t_shell *shell);

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

int			is_param_start(const char *cur);

t_lxstate	handle_general(t_lexer *lx);

t_lxstate	handle_in_single_q(t_lexer *lx);
t_lxstate	handle_in_double_q(t_lexer *lx);
t_lxstate	handle_param(t_lexer *lx);
t_lxstate	handle_finish(t_lexer *lx);

void		free_segments(t_seg **seg);
void		free_tok(t_tok **tok);
void		free_tokens(t_tok **tok);

int			tokenize(t_shell *shell);

// parser

int			is_redirtok(t_tok *tok);
void		consume_tok(t_tok **cur);
char		*get_text_tok(t_tok *tok);

t_redir		*new_redir(t_redirtype type, t_tok *word);
t_redir		*new_redir_from_tok(t_tok *op, t_tok *word);
void		redir_push(t_redirs *list, t_redir *redir);
int			collect_redir(t_shell *shell, t_tok **cur, t_redirs *list);

int			collect_redirs(t_shell *shell, t_tok **cur, t_redirs *redirs);

t_ast		*new_cmd_leaf(void);
void		collect_cmd(t_tok **cur, t_ast *cmd, t_tok **last_word);
int			parse_cmd(t_shell *shell, t_tok **cur, t_ast **out);
t_ast		*new_subsh_node(t_ast *child);
int			parse_subsh(t_shell *shell, t_tok **cur, t_ast **out);

int			parse_cmd_subsh(t_shell *shell, t_tok **cur, t_ast **out);

t_ast		*new_op_node(t_asttype type, t_ast *left, t_ast *right);
int			parse_pipe(t_shell *shell, t_tok **cur, t_ast **out);
int			parse_and_or(t_shell *shell, t_tok **cur, t_ast **out);

void		free_redirslst(t_redir **head);
void		free_redirs(t_redirs *list);
void		free_ast_cmd_parse(t_ast **ast);
void		free_ast_parse(t_ast **ast);

int			parse_ast(t_shell *shell);

// expansion

t_atom		*new_atom(t_atomtype type, const char *value, size_t len);
int			new_builder(t_expand *build);
int			new_argv_push(t_argv *argv, char *value);
int			new_argvdup_push(t_argv *argv, char *value);
char		**convert_argv_to_array(t_argv *argv);

int			append_atom(t_expand *b, const char *value, size_t l, t_atomtype t);
t_atomtype	atom_peek(t_atom *atom, size_t offset, char *out);
void		atom_advance(t_atom **atom, size_t *offset);

int			multi_param(t_shell *shell, t_expand *build, char *val);
int			solve_param(t_shell *shell, t_seg *param, t_expand *build);

int			expand_redir(t_shell *shell, t_redir *redir);
int			expand_redirs(t_shell *shell, t_redirs *redirs);

int			expand_wildcards(t_shell *shell, t_builder *builder, t_argv *argv);

void		free_atoms(t_atom **atoms);
void		free_builder(t_builder **builder);
void		free_builders(t_builder **builders);
void		free_argv(t_argv *argv);
void		free_t_expand(t_expand *build);

void		free_exp_redir(t_redir **redir);
void		free_exp_redirslst(t_redir **head);
void		free_exp_redirs(t_redirs *redirs);
void		free_exp_ast_cmd(t_ast **ast);
void		free_exp_ast(t_ast **ast);

char		*literal_expansion(t_tok *word);
char		*simple_expansion(t_tok *word);

int			expand_cmd(t_shell *shell, t_cmd *cmd);
int			expand_subsh(t_shell *shell, t_subsh *subsh);

int			build_literals(t_shell *shell, t_builder *builder, t_argv *argv);
int			expansion(t_shell *shell, t_tok *tok, t_argv *argv, int is_assign);

// env functions

char		**get_envp_shell(t_shell *shell);
t_env		*create_env_node(char *full, char *value);
char		*get_env_value(t_env_list *env_list, const char *key);
void		env_list_push(t_env_list *env_list, t_env *node);
void		free_env_list(t_env_list *env_list);

// execution

int			execute_ast(t_shell *shell, t_ast *node);
int			exec_ast_pipe(t_shell *shell, t_ast *node);
int			run_builtin_external(t_shell *shell, t_cmd *cmd);
int			exec_ast_cmd(t_shell *shell, t_cmd *cmd);
int			is_builtin(char *cmd);
void		debug_builtin(const char *cmd);

int			resolve_cmd_path(t_shell *shell, char **dst, const char *cmd);
int			get_cmd_path(t_shell *s, char **dst, const char *cmd, char **paths);

// signals

void		set_g_signum(int s);

void		signals_repl(t_shell *shell);
void		signals_wait(t_shell *shell);
void		signals_exec(t_shell *shell);

// main helper functions

int			init_shell(t_shell *shell, char *envp[]);

void		set_last_status(t_shell *shell, long long status);

void		repl(t_shell *shell);

// !debug: delete file and functions
int			debug_tokenizer(t_shell *shell);
int			debug_parser(t_shell *shell);

#endif
