/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: migugar2 <migugar2@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/16 11:50:24 by migugar2          #+#    #+#             */
/*   Updated: 2025/09/11 18:46:32 by migugar2         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	free_atoms(t_atom **atoms)
{
	t_atom	*current;
	t_atom	*next;

	if (atoms == NULL || *atoms == NULL)
		return ;
	current = *atoms;
	while (current != NULL)
	{
		next = current->next;
		free(current);
		current = next;
	}
	*atoms = NULL;
}

void	free_builder(t_builder **builder)
{
	if (builder == NULL || *builder == NULL)
		return ;
	free_atoms(&(*builder)->head);
	free(*builder);
	*builder = NULL;
}

void	free_builders(t_builder **builders)
{
	t_builder	*current;
	t_builder	*next;

	if (builders == NULL || *builders == NULL)
		return ;
	current = *builders;
	while (current != NULL)
	{
		next = current->next;
		free_builder(&current);
		current = next;
	}
	*builders = NULL;
}

void	free_t_expand(t_expand *build)
{
	if (build == NULL)
		return ;
	free_builders(&build->head);
	build->tail = NULL;
	if (build->last_status_str != NULL)
	{
		free(build->last_status_str);
		build->last_status_str = NULL;
	}
}

void	free_argv(t_argv *argv)
{
	if (argv == NULL)
		return ;
	ft_lstclear(&argv->head, free);
	argv->tail = NULL;
}
