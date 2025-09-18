/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: migugar2 <migugar2@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/15 17:49:59 by gomandam          #+#    #+#             */
/*   Updated: 2025/09/18 12:19:22 by migugar2         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	free_env_list(t_env_list *env_list)
{
	t_env	*cur;
	t_env	*next;

	cur = env_list->head;
	while (cur != NULL)
	{
		free(cur->full);
		cur->full = NULL;
		cur->value = NULL;
		next = cur->next;
		free(cur);
		cur = next;
	}
	env_list->head = NULL;
	env_list->tail = NULL;
	env_list->size = 0;
	if (env_list->envp != NULL)
	{
		free(env_list->envp);
		env_list->envp = NULL;
	}
}

int	init_envp(t_shell *shell, char *envp[])
{
	t_env	*cur;

	shell->env_list.head = NULL;
	shell->env_list.tail = NULL;
	shell->env_list.size = 0;
	shell->env_list.envp = NULL;
	while (envp[shell->env_list.size] != NULL)
	{
		cur = malloc(sizeof(t_env));
		if (cur == NULL)
			return (perror_malloc(shell), free_env_list(&shell->env_list), 1);
		cur->full = ft_strdup(envp[shell->env_list.size]);
		if (cur->full == NULL)
			return (perror_malloc(shell), free(cur),
				free_env_list(&shell->env_list), 1);
		cur->value = ft_strchr(cur->full, '=') + 1;
		cur->next = NULL;
		env_list_push(&shell->env_list, cur);
	}
	return (0);
}

int	init_shell(t_shell *shell, char *envp[])
{
	shell->line = NULL;
	shell->tokens = NULL;
	shell->ast = NULL;
	shell->last_status = 0;
	shell->finished = 0;
	shell->interactive = isatty(STDIN_FILENO);
	if (init_envp(shell, envp) == 1)
		return (1);
	if (shell->interactive)
		ft_putendl_fd("Welcome to MiniYeska!", STDOUT_FILENO);
	return (0);
}
