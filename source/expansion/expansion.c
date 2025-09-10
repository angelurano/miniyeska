/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansion.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: migugar2 <migugar2@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/12 23:00:52 by migugar2          #+#    #+#             */
/*   Updated: 2025/09/03 19:15:40 by migugar2         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static size_t	get_len_litbuilder(t_builder *builder)
{
	size_t	len;
	t_atom	*cur;

	len = 0;
	cur = builder->head;
	while (cur != NULL)
	{
		len += cur->len;
		cur = cur->next;
	}
	return (len);
}

int	build_literals(t_shell *shell, t_builder *builder, t_argv *argv)
{
	size_t	total_len;
	size_t	i;
	t_atom	*acur;
	char	*joined;

	total_len = get_len_litbuilder(builder);
	joined = malloc(sizeof(char) * (total_len + 1));
	if (joined == NULL)
		return (perror_malloc(), 1);
	acur = builder->head;
	i = 0;
	while (acur != NULL)
	{
		if (acur->type == ATOM_LIT)
			ft_strlcpy(joined + i, acur->value, acur->len + 1);
		else if (acur->type == ATOM_WILD)
			ft_memset(joined + i, '*', acur->len);
		i += acur->len;
		acur = acur->next;
	}
	joined[total_len] = '\0';
	if (new_argv_push(argv, joined) == 1)
		return (perror_malloc(), free(joined), 1);
	return (0);
	(void)shell;
}

int	build_expansion(t_shell *shell, t_expand *build, t_argv *argv)
{
	t_builder	*next;

	while (build->head != NULL)
	{
		if (build->head->flags & BUILDF_WILD
			&& !(build->tail->flags & BUILDF_EQ))
		{
			if (expand_wildcards(shell, build->head, argv) == 1)
				return (1);
		}
		else
		{
			if (build_literals(shell, build->head, argv) == 1)
				return (1);
		}
		next = build->head->next;
		free_builder(&build->head);
		build->head = next;
	}
	return (0);
}

int	get_builders(t_shell *shell, t_tok *tok, t_expand *build)
{
	t_seg		*cur;

	if (new_builder(build) == 1)
		return (1);
	cur = tok->seg_head;
	while (cur != NULL)
	{
		if (cur->type == SEG_TEXT)
		{
			if (append_atom(build, cur->slice.begin, cur->slice.len,
					ATOM_LIT) == 1)
				return (1);
		}
		else if (cur->type == SEG_WILDCARD)
		{
			if (append_atom(build, NULL, 1, ATOM_WILD) == 1)
				return (1);
		}
		else if (cur->type == SEG_PARAM && solve_param(shell, cur, build) == 1)
			return (1);
		cur = cur->next;
	}
	return (0);
}

int	expansion(t_shell *shell, t_tok *tok, t_argv *argv, int is_assign)
{
	t_expand	build;

	build.head = NULL;
	build.tail = NULL;
	build.last_status = NULL;
	build.is_assign = is_assign;
	if (get_builders(shell, tok, &build) == 1)
		return (perror_malloc(), free_t_expand(&build), 1);
	if (build.head->head == NULL)
		return (free_t_expand(&build), 0);
	if (build_expansion(shell, &build, argv) == 1)
		return (free_t_expand(&build), 1);
	free_t_expand(&build);
	return (0);
}
