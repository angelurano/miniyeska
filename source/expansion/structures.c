/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   structures.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: migugar2 <migugar2@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/16 11:57:45 by migugar2          #+#    #+#             */
/*   Updated: 2025/09/03 18:19:43 by migugar2         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_atom	*new_atom(t_atomtype type, const char *value, size_t len)
{
	t_atom	*new;

	new = malloc(sizeof(t_atom));
	if (new == NULL)
		return (NULL);
	new->type = type;
	new->value = value;
	new->len = len;
	new->next = NULL;
	return (new);
}

int	new_builder(t_expand *build)
{
	t_builder	*builder;

	builder = malloc(sizeof(t_builder));
	if (builder == NULL)
		return (1);
	builder->head = NULL;
	builder->tail = NULL;
	builder->next = NULL;
	builder->flags = BUILDF_NONE;
	if (build->is_assign)
		builder->flags |= BUILDF_ASSIGN;
	if (build->head == NULL)
		build->head = builder;
	else
		build->tail->next = builder;
	build->tail = builder;
	return (0);
}

int	new_argv_push(t_argv *argv, char *value)
{
	t_list	*new_node;

	new_node = ft_lstnew(value);
	if (new_node == NULL)
		return (1);
	if (argv->head == NULL)
		argv->head = new_node;
	else
		argv->tail->next = new_node;
	argv->tail = new_node;
	argv->argc++;
	return (0);
}

int	new_argvdup_push(t_argv *argv, char *todup)
{
	char	*copy;

	copy = ft_strdup(todup);
	if (copy == NULL)
		return (1);
	return (new_argv_push(argv, copy));
}

char	**convert_argv_to_array(t_argv *argv)
{
	char	**array;
	size_t	i;
	t_list	*next;

	array = malloc(sizeof(char *) * (argv->argc + 1));
	if (array == NULL)
		return (NULL);
	i = 0;
	while (i < argv->argc)
	{
		array[i] = argv->head->content;
		next = argv->head->next;
		ft_lstdelone(argv->head, NULL);
		argv->head = next;
		i++;
	}
	argv->tail = NULL;
	array[i] = NULL;
	return (array);
}
