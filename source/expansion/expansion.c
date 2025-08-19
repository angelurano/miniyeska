/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansion.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: migugar2 <migugar2@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/12 23:00:52 by migugar2          #+#    #+#             */
/*   Updated: 2025/08/16 17:21:55 by migugar2         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_param	*expand_param(t_shell *shell, t_seg *seg)
{
	t_param	*res;
	char	*key;

	res = new_param();
	if (res == NULL)
		return (NULL);
	if (seg->slice.len == 1 && seg->slice.begin[0] == '?')
	{
		res->value = ft_itoa(shell->last_status);
		res->len = ft_strlen(res->value);
		return (res);
	}
	key = malloc(sizeof(char) * (seg->slice.len + 1));
	if (key == NULL)
		return (free(res), NULL);
	ft_strlcpy(key, seg->slice.begin, seg->slice.len + 1);
	res->value = get_env_value(&shell->env_list, key);
	free(key);
	if (res->value == NULL)
		res->value = "";
	res->len = ft_strlen(res->value);
	return (res);
}

size_t	consume_param(t_exp *exp, t_seg *seg, char *cur)
{
	t_param	*tmp;
	size_t	len;

	tmp = exp->head;
	len = tmp->len;
	ft_strlcpy(cur, tmp->value, tmp->len + 1);
	exp->head = exp->head->next;
	free_param(&tmp, seg);
	if (exp->head == NULL)
		exp->tail = NULL;
	return (len);
}

int	expand_tok(t_exp *exp, t_tok *tok, t_argv *argv)
{
	t_seg	*cur;
	size_t	i;
	char	*expanded;

	expanded = malloc(sizeof(char) * (exp->len + 1));
	if (expanded == NULL)
		return (perror_malloc());
	i = 0;
	cur = tok->seg_head;
	while (cur != NULL)
	{
		if (cur->type == SEG_TEXT)
		{
			ft_strlcpy(&expanded[i], cur->slice.begin, cur->slice.len + 1);
			i += cur->slice.len;
		}
		else if (cur->type == SEG_PARAM)
			i += consume_param(exp, cur, &expanded[i]);
		cur = cur->next;
	}
	if (new_argv_push(argv, expanded) == 1)
		return (perror_malloc(), free(expanded), 1);
	return (0);
}

static int	get_info(t_shell *shell, t_exp *exp, t_tok *tok)
{
	t_seg	*cur;
	t_param	*param;

	cur = tok->seg_head;
	while (cur != NULL)
	{
		if (cur->type == SEG_TEXT)
			exp->len += cur->slice.len;
		else if (cur->type == SEG_PARAM)
		{
			param = expand_param(shell, cur);
			if (param == NULL)
				return (perror_malloc(),
					free_paramlst(&exp->head, &exp->tail, tok->seg_head), 1);
			param_push(&exp->head, &exp->tail, param);
			exp->len += param->len;
		}
		else if (cur->type == SEG_WILDCARD)
		{
			exp->wildcards++;
			exp->len++;
		}
		cur = cur->next;
	}
	return (0);
}

int	expansion(t_shell *shell, t_tok *tok, t_argv *argv)
{
	t_exp	exp;

	exp.head = NULL;
	exp.tail = NULL;
	exp.len = 0;
	exp.wildcards = 0;
	if (get_info(shell, &exp, tok) == 1)
		return (1);
	if (exp.wildcards > 0)
	{
		if (expand_wildcards(shell, &exp, tok, argv) == 1)
			return (free_paramlst(&exp.head, &exp.tail, tok->seg_head), 1);
	}
	else
	{
		if (expand_tok(&exp, tok, argv) == 1)
			return (free_paramlst(&exp.head, &exp.tail, tok->seg_head), 1);
	}
	return (0);
}
