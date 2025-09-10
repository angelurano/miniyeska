/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   atoms.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: migugar2 <migugar2@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/03 14:02:46 by migugar2          #+#    #+#             */
/*   Updated: 2025/09/03 18:20:51 by migugar2         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	check_assignment(t_expand *build, t_atom *atom)
{
	size_t	i;

	if (build->tail->flags & BUILDF_EQ)
		return ;
	i = 0;
	while (i < atom->len)
	{
		if (build->tail->flags & BUILDF_LEFT)
		{
			if (atom->value[i] == '=')
				build->tail->flags |= BUILDF_EQ;
		}
		else if (ft_isalpha(atom->value[i]) || atom->value[i] == '_')
			build->tail->flags |= BUILDF_LEFT;
		i++;
	}
}

static int	push_atom(t_expand *b, const char *str, size_t len, t_atomtype t)
{
	t_atom	*new;

	new = new_atom(t, str, len);
	if (new == NULL)
		return (1);
	if (b->tail->head == NULL)
		b->tail->head = new;
	else
		b->tail->tail->next = new;
	b->tail->tail = new;
	if (t == ATOM_WILD && !(b->tail->flags & BUILDF_EQ))
		b->tail->flags |= BUILDF_WILD;
	if (b->tail->flags & BUILDF_ASSIGN && t == ATOM_LIT)
		check_assignment(b, b->tail->tail);
	return (0);
}

int	append_atom(t_expand *build, const char *value, size_t len, t_atomtype type)
{
	t_atom	*next;

	if (build->tail == NULL || build->tail->flags & BUILDF_FINISH)
	{
		if (new_builder(build) == 1)
			return (1);
	}
	if (type == ATOM_WILD && build->tail->tail
		&& build->tail->tail->type == ATOM_WILD)
	{
		build->tail->tail->len += len;
		return (0);
	}
	if (type == ATOM_LIT && len == 0 && build->tail->head)
		return (0);
	if (build->tail->head && build->tail->head->type == ATOM_LIT
		&& build->tail->head->len == 0)
	{
		next = build->tail->head->next;
		if (build->tail->tail == build->tail->head)
			build->tail->tail = next;
		free(build->tail->head);
		build->tail->head = next;
	}
	return (push_atom(build, value, len, type));
}

t_atomtype	atom_peek(t_atom *atom, size_t offset, char *out)
{
	while (atom && offset >= atom->len)
	{
		offset -= atom->len;
		atom = atom->next;
	}
	if (atom == NULL)
		return (ATOM_END);
	else if (atom->type == ATOM_WILD)
	{
		if (out != NULL)
			*out = '*';
		return (ATOM_WILD);
	}
	else if (atom->type == ATOM_LIT)
	{
		if (out != NULL)
			*out = atom->value[offset];
		return (ATOM_LIT);
	}
	return (ATOM_END);
}

void	atom_advance(t_atom **atom, size_t *offset)
{
	t_atom	*cur;
	size_t	k;

	cur = *atom;
	k = *offset;
	while (cur && k >= cur->len)
	{
		k -= cur->len;
		cur = cur->next;
	}
	if (cur == NULL)
	{
		*atom = NULL;
		*offset = k;
		return ;
	}
	k++;
	if (k >= cur->len)
	{
		cur = cur->next;
		k = 0;
	}
	*offset = k;
	*atom = cur;
}
