/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe_exec.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gomandam <gomandam@student.42madrid>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/22 00:24:27 by gomandam          #+#    #+#             */
/*   Updated: 2025/08/29 23:53:05 by gomandam         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <sys/wait.h>
#include <unistd.h>

// waits for process ids, returns exit status of right child process
// st1 & st2: storage exit status, overwritten by waitpid even initialized 0
static int	wait_pipe(pid_t p1, pid_t p2)
{
	int	st1;
	int	st2;

	waitpid(p1, &st1, 0);
	waitpid(p2, &st2, 0);
	if (WIFEXITED(st2))
		return (WEXITSTATUS(st2));
	if (WIFSIGNALED(st2))
		return (128 + WTERMSIG(st2));
	return (1);
}

// local function for exec_ast_pipe();
// setup child pipe, execute subtree then exit
static void	child_pipe_end(t_shell *shell, t_ast *node, int fd[2], int stdio)
{
	int	in_fd;
	int	close_fd;

	ft_putstr_fd("DEBUG: entered child_pipe_end at pipe_exec.c\n", 2);

	in_fd = -1;
	close_fd = -1;
	if (stdio == STDOUT_FILENO)
	{
		in_fd = fd[1];
		close_fd = fd[0];
	}
	else if (stdio == STDIN_FILENO)
	{
		in_fd = fd[0];
		close_fd = fd[1];
	}
	if (close_fd >= 0)
		close(close_fd);
	if (in_fd >= 0)
	{
		dup2(in_fd, stdio);
		close(in_fd);
	}
	_exit(execute_ast(shell, node));
}

// fork the child process for a pipe
// All commands in the pipeline execute in parallel,
// 	regardless of pipeline depth or tree structure.
static pid_t	pipe_fork(t_shell *shell, t_ast *node, int fd[2], int stdio)
{
	pid_t	pid;

	pid = fork();
	if (pid == 0)
		child_pipe_end(shell, node, fd, stdio);
	return (pid);
}

// Per-node: create pipe, fork: L | R, close fds, execute concurrent, return right status.
// fork: left writes fd[1] and fork: right reads fd[0]
// Each stage (AST_PIPE) immediately forks both left and right children.
int	exec_ast_pipe(t_shell *shell, t_ast *node)
{
	int		fd[2];
	pid_t	pid_l;
	pid_t	pid_r;

	fd[0] = -1;
	fd[1] = -1;
	if (!node || !node->u_data.op.left || !node->u_data.op.right)
		return (1);
	if (pipe(fd) < 0)
		return (perror("minishell: pipe"), 1);
	pid_l = pipe_fork(shell, node->u_data.op.left, fd,
			STDOUT_FILENO);
	pid_r = pipe_fork(shell, node->u_data.op.right, fd,
			STDIN_FILENO);
	if (fd[0] >= 0)
		close(fd[0]);
	if (fd[1] >= 0)
		close(fd[1]);
	return (wait_pipe(pid_l, pid_r));
}

