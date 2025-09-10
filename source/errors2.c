/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   errors2.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: migugar2 <migugar2@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/22 00:24:27 by gomandam          #+#    #+#             */
/*   Updated: 2025/09/10 02:19:39 by migugar2         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	perror_cmdnotfound(t_shell *shell, const char *cmd)
{
	size_t	cmd_len;

	cmd_len = ft_strlen(cmd);
	write(STDERR_FILENO, "minishell: command not found: ", 30);
	write(STDERR_FILENO, cmd, cmd_len);
	write(STDERR_FILENO, "\n", 1);
	if (shell)
		shell->last_status = 127;
	return (1);
}

