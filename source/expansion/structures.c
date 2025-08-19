/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   structures.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: migugar2 <migugar2@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/16 11:57:45 by migugar2          #+#    #+#             */
/*   Updated: 2025/08/16 17:20:55 by migugar2         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_param	*new_param(void)
{
	t_param	*param;

	param = malloc(sizeof(t_param));
	if (param == NULL)
		return (NULL);
	param->value = NULL;
	param->len = 0;
	param->next = NULL;
	return (param);
}

void	param_push(t_param **head, t_param **tail, t_param *new_param)
{
	if (head == NULL || tail == NULL || new_param == NULL)
		return ;
	if (*head == NULL)
		*head = new_param;
	else
		(*tail)->next = new_param;
	*tail = new_param;
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
