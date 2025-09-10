/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wildcards.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: migugar2 <migugar2@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/16 12:08:36 by migugar2          #+#    #+#             */
/*   Updated: 2025/09/03 18:44:03 by migugar2         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	wildcard_recursive(const char *str, t_atom *atom, size_t off)
{
	char	c;
	t_atom	*newatom;
	size_t	newoff;

	if (atom_peek(atom, off, &c) == ATOM_END || c == '\0')
		return (*str == '\0');
	if (atom_peek(atom, off, NULL) == ATOM_WILD)
	{
		newatom = atom;
		newoff = off;
		while (atom_peek(newatom, newoff, NULL) == ATOM_WILD)
			atom_advance(&newatom, &newoff);
		if (wildcard_recursive(str, newatom, newoff))
			return (1);
		while (*str)
		{
			if (wildcard_recursive(str + 1, newatom, newoff))
				return (1);
			str++;
		}
		return (0);
	}
	if (*str == '\0' || *str != c)
		return (0);
	return (atom_advance(&atom, &off), wildcard_recursive(str + 1, atom, off));
}

int	wildcard_match(const char *str, t_atom **start, size_t *offset)
{
	char	c;

	if (str[0] == '.' && (str[1] == '\0' || (str[1] == '.' && str[2] == '\0')
			|| (atom_peek(*start, *offset, &c) && c != '.')))
		return (0);
	return (wildcard_recursive(str, *start, *offset));
}

int	start_wildcard(t_builder *builder, t_atom **start, size_t *offset)
{
	t_atom	*cur;
	size_t	k;
	char	c;

	cur = builder->head;
	k = 0;
	while (atom_peek(cur, k, &c) && c == '.'
		&& atom_peek(cur, k + 1, &c) && c == '/')
	{
		atom_advance(&cur, &k);
		atom_advance(&cur, &k);
		while (atom_peek(cur, k, &c) && c == '/')
			atom_advance(&cur, &k);
	}
	*start = cur;
	*offset = k;
	while (atom_peek(cur, k, &c))
	{
		if (c == '/')
			return (1);
		atom_advance(&cur, &k);
	}
	return (0);
}

int	wildcard_solve(t_shell *shell, t_builder *builder, t_argv *argv)
{
	DIR				*cur;
	struct dirent	*entry;
	t_atom			*start;
	size_t			offset;

	if (start_wildcard(builder, &start, &offset) == 1)
		return (0);
	cur = opendir(".");
	if (cur == NULL)
	{
		if (errno == ENOMEM)
			return (perror_malloc(), 1);
		return (errno = 0, 0);
	}
	entry = readdir(cur);
	while (entry != NULL)
	{
		if (wildcard_match(entry->d_name, &start, &offset))
		{
			if (new_argvdup_push(argv, entry->d_name) == 1)
				return (perror_malloc(), closedir(cur), 1);
		}
		entry = readdir(cur);
	}
	return (closedir(cur), 0);
	(void)shell;
}

int	expand_wildcards(t_shell *shell, t_builder *builder, t_argv *argv)
{
	size_t	initial_argc;

	initial_argc = argv->argc;
	if (wildcard_solve(shell, builder, argv) == 1)
		return (1);
	if (argv->argc == initial_argc)
	{
		if (build_literals(shell, builder, argv) == 1)
			return (1);
	}
	return (0);
}
