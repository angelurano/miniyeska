/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: migugar2 <migugar2@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/15 18:43:52 by migugar2          #+#    #+#             */
/*   Updated: 2025/08/15 19:11:06 by migugar2         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// TODO: if ft_strchr(full, '=') is NULL, will not execute this function, else, execute this function with the pointer + 1
t_env	*create_env_node(char *full, char *value)
{
	t_env	*node;

	node = malloc(sizeof(t_env));
	if (node == NULL)
		return (NULL);
	node->full = full;
	node->value = value;
	node->next = NULL;
	return (node);
}

void	env_list_push(t_env_list *env_list, t_env *node)
{
	if (env_list->head == NULL)
		env_list->head = node;
	else
		env_list->tail->next = node;
	env_list->tail = node;
	env_list->size++;
	if (env_list->envp != NULL)
	{
		free(env_list->envp);
		env_list->envp = NULL;
	}
}

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
