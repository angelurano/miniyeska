/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   param.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: migugar2 <migugar2@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/25 20:28:03 by migugar2          #+#    #+#             */
/*   Updated: 2025/09/11 18:46:52 by migugar2         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	multi_spec_char(t_expand *build, char *val, size_t *i, size_t *strt)
{
	if (!ft_isspace(val[*i]) && val[*i] != '*')
		return (0);
	if (*i > *strt
		&& append_atom(build, &val[*strt], *i - *strt, ATOM_LIT) == 1)
		return (1);
	if (ft_isspace(val[*i]))
	{
		if (build->tail->head != NULL)
			build->tail->flags |= BUILDF_FINISH;
		while (ft_isspace(val[*i]))
			(*i)++;
	}
	else if (val[*i] == '*')
	{
		*strt = *i;
		while (val[*i] == '*')
			(*i)++;
		if (*i > *strt && append_atom(build, NULL, *i - *strt, ATOM_WILD) == 1)
			return (1);
	}
	*strt = *i;
	return (0);
}

int	multi_param(t_shell *shell, t_expand *build, char *val)
{
	size_t	i;
	size_t	strt;

	i = 0;
	strt = 0;
	while (val[i])
	{
		if (ft_isspace(val[i]) || val[i] == '*')
		{
			if (multi_spec_char(build, val, &i, &strt) == 1)
				return (1);
		}
		else
			i++;
	}
	if (i > strt && val[strt] && !ft_isspace(val[strt]))
		return (append_atom(build, &val[strt], i - strt, ATOM_LIT));
	return (0);
	(void)shell;
}

int	solve_param(t_shell *shell, t_seg *param, t_expand *build)
{
	char	*key;
	char	*value;

	if (param->slice.len == 1 && param->slice.begin[0] == '?')
	{
		if (build->last_status_str == NULL)
			build->last_status_str = ft_itoa(shell->last_status);
		if (build->last_status_str == NULL)
			return (1);
		return (append_atom(build, build->last_status_str,
				ft_strlen(build->last_status_str), ATOM_LIT));
	}
	key = malloc(sizeof(char) * (param->slice.len + 1));
	if (key == NULL)
		return (1);
	ft_strlcpy(key, param->slice.begin, param->slice.len + 1);
	value = get_env_value(&shell->env_list, key);
	free(key);
	if (value == NULL || value[0] == '\0')
		return (0);
	if (param->flags & SEGF_QUOTED || build->tail->flags & BUILDF_EQ)
		return (append_atom(build, value, ft_strlen(value), ATOM_LIT));
	return (multi_param(shell, build, value));
}
