/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_exec.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gomandam <gomandam@student.42madrid>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/22 00:24:27 by gomandam          #+#    #+#             */
/*   Updated: 2025/09/09 21:25:55 by gomandam         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// checks if 'built-in' with "if" conditional statments, or control flow
// returns 1 if argv[0] matches a builtin, else 0.
int	is_builtin(char *cmd)
{
	if (!cmd)
		return (0);
	if (!ft_strcmp(cmd, "echo"))
		return (1);
	if (!ft_strcmp(cmd, "cd"))
		return (1);
	if (!ft_strcmp(cmd, "pwd"))
		return (1);
	if (!ft_strcmp(cmd, "export"))
		return (1);
	if (!ft_strcmp(cmd, "unset"))
		return (1);
	if (!ft_strcmp(cmd, "env"))
		return (1);
	if (!ft_strcmp(cmd, "exit"))
		return (1);
	return (0);
}

/* //  executes if it is a built-in, assuming builtins are working
// following: exit, cd, echo, env, export, unset, pwd
// !TO DO: rename the built-ins ft_*() for proper naming functionality
int	run_builtin_external(t_shell *shell, char *argv[])
{
	if (!argv || !argv[0])
		return (1);
	if (!ft_strcmp(argv[0], "echo"))
		return (ft_echo(argv));		// char **argv
	if (!ft_strcmp(argv[0], "cd"))
		return (ft_cd(shell, argv));	// t_shell *shell, char **argv
	if (!ft_strcmp(argv[0], "pwd"))
		return (ft_pwd());		// void
	if (!ft_strcmp(argv[0], "export"))
		return (ft_export(shell, argv)); // ! TO DO: rebuild export builtin
	if (!ft_strcmp(argv[0], "unset"))
		return (ft_unset(shell, argv));	// check t_env_list
	if (!ft_strcmp(argv[0], "env"))
		return (ft_env(shell));		// check t_env_list *env_list
	if (!ft_strcmp(argv[0]), "exit")
		return (ft_exit(shell, argv));
	return (1);
}*/
// DEBUGGER: Temporary functions to check the access of builtins
void	debug_builtin(const char *cmd)
{
	ft_putstr_fd("DEBUG: inside debug_builtin(); at builtin_exec.c\n", 2);

	if (!cmd)
	{
		printf("No command given\n");
		return ;
	}
	if (!ft_strcmp(cmd, "echo"))
		printf("Temporary: execute builtin: echo\n");
	else if (!ft_strcmp(cmd, "cd"))
		printf("Temporary: execute builtin: cd\n");
	else if (!ft_strcmp(cmd, "pwd"))
		printf("Temporary: execute builtin: pwd\n");
	else if (!ft_strcmp(cmd, "export"))
		printf("Temporary: execute builtin: export\n");
	else if (!ft_strcmp(cmd, "unset"))
		printf("Temporary: execute builtin: unset\n");
	else if (!ft_strcmp(cmd, "env"))
		printf("Temporary: execute builtin: env\n");
	else if (!ft_strcmp(cmd, "exit"))
		printf("Temporary: execute builtin: exit\n");
	else
		printf("Not a builtin: %s\n", cmd);
}
