/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: migugar2 <migugar2@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/20 00:02:21 by migugar2          #+#    #+#             */
/*   Updated: 2025/07/26 02:44:46 by migugar2         ###   ########.fr       */
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

// extern volatile sig_atomic_t	g_signum;

# define MINI_PROMPT "MINI> $ " // TODO

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

#endif
