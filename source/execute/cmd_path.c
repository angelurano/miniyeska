/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_path.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: migugar2 <migugar2@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/22 00:24:27 by gomandam          #+#    #+#             */
/*   Updated: 2025/09/11 18:47:42 by migugar2         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	join_path_cmd(char **dst_path, const char *dir, const char *cmd)
{
	size_t	dir_len;
	size_t	cmd_len;

	dir_len = ft_strlen(dir);
	cmd_len = ft_strlen(cmd);
	*dst_path = malloc(sizeof(char) * (dir_len + 1 + cmd_len + 1));
	if (*dst_path == NULL)
		return (perror_malloc(NULL)); // TODO
	ft_strlcpy(*dst_path, dir, dir_len + 1);
	(*dst_path)[dir_len] = '/';
	ft_strlcpy(*dst_path + dir_len + 1, cmd, cmd_len + 1);
	return (0);
}

int	get_cmd_path(t_shell *shell, char **dst, const char *cmd, char **paths)
{
	char	*tmp;
	size_t	i;

	i = 0;
	while (paths != NULL && paths[i] != NULL)
	{
		if (join_path_cmd(&tmp, paths[i], cmd) == 1)
			return (1);
		if (access(tmp, F_OK) == 0)
		{
			*dst = tmp;
			return (0);
		}
		ft_freestr(&tmp);
		i++;
	}
	return (perror_cmdnotfound(NULL, cmd)); // TODO
	(void)shell;
}

int	resolve_cmd_path(t_shell *shell, char **dst, const char *cmd)
{
	t_env	*cur;
	char	**paths;

	cur = shell->env_list.head;
	paths = NULL;
	while (cur != NULL)
	{
		if (ft_strncmp(cur->full, "PATH=", 5) == 0)
		{
			paths = ft_split(cur->value, ':');
			if (paths == NULL)
				return (perror_malloc(NULL)); // TODO
			break ;
		}
		cur = cur->next;
	}
	if (paths == NULL)
		return (perror_cmdnotfound(NULL, cmd)); // TODO
	if (get_cmd_path(shell, dst, cmd, paths) == 1)
		return (ft_freestrarr(&paths), 1);
	ft_freestrarr(&paths);
	return (0);
}
