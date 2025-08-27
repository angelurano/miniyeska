/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_exec.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gomandam <gomandam@student.42madrid>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/22 00:24:27 by gomandam          #+#    #+#             */
/*   Updated: 2025/08/27 15:29:25 by gomandam         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"
#include <sys/wait.h>

// Fork and execute external cmd, wait for completion. Returns exit status
static int	run_external(t_shell *shell, t_cmd *cmd)
{
	int	status;
	pid_t	pid;

	pid = fork();
	if (pid < 0)
		return (perror("minishell: fork"), 1);
	if (pid == 0)
	{
		execve(cmd->u_data.argv[0], cmd->u_data.argv, shell->env_list.envp);
		perror("minishell: execve");
		_exit(127);
	}
	waitpid(pid, &status, 0);
	if (WIFEXITED(status))
		return (WEXITSTATUS(status));
	if (WIFSIGNALED(status))
		return (128 + WTERMSIG(status));
	return (1);	// ? Return PID? 
}

// Decide builtin or external, execute builtin directly, and 
// external is via fork/execve. Then, return cmd exit status
int	run_builtin_external(t_shell *shell, t_cmd *cmd)
{
	if (!cmd->u.data.argv || !cmd->u_data.argv[0])
		return (0);
	if (is_builtin(cmd->u_data.argv[0]))
		return (run_builtin(shell, cmd->u_data.argv));
	return (run_external(shell, cmd));
}

// expand & execute cmd AST node. returns exit status
int	exec_ast_cmd(t_shell *shell, t_cmd *cmd)
{
	if (expand_cmd(shell, cmd) != 0)
		return (1);
	return (run_builtin_external(shell, cmd));
}
