/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gomandam <gomandam@student.42madrid>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/24 02:04:11 by gomandam          #+#    #+#             */
/*   Updated: 2025/08/16 17:39:16 by gomandam         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/*	Unset values & attributes of variables.
 *	Removes access from the current shell session.
 *	Does NOT affect the parent of the shell.	*/

#include "../../include/minishell.h"

// Find env node by key
static int	find_env_node(t_env_list *env_list, const char *key,
	t_env **found, t_env **prev)
{
	t_env	*cur;
	size_t	keylen;

	*prev = NULL;
	cur = env_list->head;
	keylen = ft_strlen(key);
	while (cur)
	{
		if (ft_strncmp(cur->full, key, keylen) == 0
			&& cur->full[keylen] == '=')
		{
			*found = cur;
			return (1);
		}
		*prev = cur;
		cur = cur->next;
	}
	*found = NULL;
	return (0);
}

//	Remove+free env node from list. Pointer updated, Node freed. 
static void	remove_env_node(t_env_list *env_list, t_env *node, t_env *prev)
{
	if (prev)
		prev->next = node->next;
	else
		env_list->head = node->next;
	if (env_list->tail == node)
		env_list->tail = prev;
	free(node->full);
	free(node);
	env_list->size--;
	if (env_list->envp)	// free cache
	{
		free(env_list->envp);
		env_list->envp = NULL;
	}
}

/*
	unset: implements the 'unset' built-in.
	argv[0] = "unset", argv[1..n] = var names to unset
	Returns 1 if any variable was removed, 0 otherwise.	*/
int	unset(t_env_list *env_list, char *argv[])
{
	int	removed;
	int	i;
	t_env	*node;
	t_env	*prev;

	if (!env_list || !argv || !argv[1])
		return (0);
	removed = 0;
	i = 1;
	while (argv[i])
	{
		if (find_env_node(env_list, argv[i], &node, &prev))
		{
			remove_env_node(env_list, node, prev);
			removed = 1;
		}
		i++;
	}
	return (removed);
}

/*
int	unset(char *argv[], char *envp[])
{
	int		a;
	int		b;
	char	*var;

	a = 0;
	if (!argv[0])
		return (0);
	var = argv[1];
	while (envp[a])
	{
		if (ft_strncmp(envp[a], var, ft_strlen(var)) == 0
			&& envp[a][ft_strlen(var) == '='])
		{
			b = a;
			while (envp[b])
			{
				envp[b] = envp[b + 1];
				b++;
			}
			return (1);
		}
		a++;
	}
	printf("minishell: unset: variable %s not found\n", var);
	return (1);
}
*/
