/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_exec.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gomandam <gomandam@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/22 00:24:27 by gomandam          #+#    #+#             */
/*   Updated: 2025/09/18 01:33:32 by gomandam         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"
#include "../../libft/libft.h"
#include <sys/wait.h>
#include <fcntl.h>

// TODO: last_status write
// fork and execute external cmd, wait for completion. Returns exit status
// 127 (manual) Utility to be executed was not found.
// waitpid(pid, &status, 0); for single CMD, not pipe. Get exit code, no zombies
static int	run_external(t_shell *shell, t_cmd *cmd)
{
	int	status;
	pid_t	pid;
	char	*resolved;

	if (ft_strchr(cmd->u_data.argv[0], '/') == NULL)
	{
		resolved = NULL;
		if (resolve_cmd_path(shell, &resolved, cmd->u_data.argv[0]) == 1)
			return (127);
		free(cmd->u_data.argv[0]);
		cmd->u_data.argv[0] = resolved;
	}
	pid = fork();
	if (pid < 0)
		return (perror("minishell: fork"), 1);
	if (pid == 0)
	{
		signals_exec(shell);
		execve(cmd->u_data.argv[0], cmd->u_data.argv, get_envp_shell(shell));
		perror("minishell: execve");
		_exit(127);
	}
	signals_wait(shell);
	waitpid(pid, &status, 0);
	signals_repl(shell);
	if (WIFEXITED(status))
		return (WEXITSTATUS(status));
	if (WIFSIGNALED(status))
	{
		int sig = WTERMSIG(status);
		if (sig == SIGQUIT)
		{
			if (__WCOREDUMP(status))
				write(STDOUT_FILENO, "Quit (core dumped)\n", 19);
			else
				write(STDOUT_FILENO, "Quit\n", 5);
		}
		return (128 + WTERMSIG(status));
	}
	return (1);
}

// Decide builtin or external, execute builtin directly, and
// external is via fork/execve. Then, return cmd exit status
// TO DO: review 'unset' but so far it is okay

int	run_builtin_external(t_shell *shell, t_cmd *cmd)
{
	ft_putstr_fd("DEBUG: entered the run_builtin_external(); at cmd_exec.c\n", 2);

//	(void)shell; 					// remove after debug
	if (!cmd->u_data.argv || !cmd->u_data.argv[0])
		return (0);
	if (is_builtin(cmd->u_data.argv[0]))
	{
		ft_putstr_fd("DEBUG: inside is_builtin() -> builtin fx\n", 2);
		if (!ft_strcmp(cmd->u_data.argv[0], "pwd"))
			return (ft_pwd());
		if (!ft_strcmp(cmd->u_data.argv[0], "env"))
			return (ft_env(&shell->env_list));
		if (!ft_strcmp(cmd->u_data.argv[0], "unset"))
			return (ft_unset(&shell->env_list, cmd->u_data.argv));
		if (!ft_strcmp(cmd->u_data.argv[0], "echo"))
			return (ft_echo(cmd));			// since ft_echo(t_cmd *cmd)
		if (!ft_strcmp(cmd->u_data.argv[0], "exit"))
			return (ft_exit(shell, cmd->u_data.argv));
		if (!ft_strcmp(cmd->u_data.argv[0], "export"))
			return (ft_export(shell, cmd->u_data.argv));
	}
	return (run_external(shell, cmd));
//	if (is_builtin(cmd->u_data.argv[0]))	// remove codesnippet after debug
//		return ((debug_builtin(cmd->u_data.argv[0])), 0);
//	return (run_external(shell, cmd));	// remove after debug
}

// expand & execute cmd AST node. returns exit status
int	exec_ast_cmd(t_shell *shell, t_cmd *cmd)
{
	ft_putstr_fd("DEBUG: Entered exec_ast_cmd();\n", 2);

	if (expand_cmd(shell, cmd) != 0)
		return (1);
	return (run_builtin_external(shell, cmd));
}
