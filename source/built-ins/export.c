/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gomandam <gomandam@student.42madrid>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/24 02:04:11 by gomandam          #+#    #+#             */
/*   Updated: 2025/08/16 15:34:35 by gomandam         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/*
	set environment variables -> available to child process
	e.g. if 'unset' a function, 'export' to access
 */

#include "../../include/minishell.h"

// Print single env var in the correct format with quotes around values.
void	print_export_var(char *var)
{
	char	*equals;
	int		i;

	if (!var)
		return ;
	printf("declare -x ");
	equals = ft_strchr(var, '=');
	if (equals)
	{			/* Print all up to equals sign */
		i = 0;
		while (var + i != equals)
		{
			printf("%c", var[i]);
			i++;
		}		/* Print equals sign followed by quoted value */
		printf("=\"");
		printf("%s", equals + 1);
		printf("\"");
	}
	else
		printf("%s", var);
	printf("\n");
}

//  Checks if a variable belongs to the current character group processing
int	matches_current_char(char *var, char current)
{
	if (!var || !*var)
		return (0);
	if (*var == current)	// 1st char matches current alphabetical position
		return (1);
	return (0);
}

/*	First processes all variables starting with letters (A-Z, then a-z)
	Handles any remaining variables start w/ non-alphabetic character	
*/
void	export_no_args(char **envp)
{
	int	i;
	char	chr;

	if (!envp)
		return;		/* start with uppercase letters */
	chr = 'A';
	while ((chr >= 'A' && chr <= 'Z') || (chr >= 'a' && chr <= 'z'))
	{
		i = 0;
		while (envp[i] != NULL)
		{
			if (matches_current_char(envp[i], chr))
				print_export_var(envp[i]);
			i++;
		}		/* move to next letter */
		chr++;
		if (chr == 'Z' + 1)
			chr = 'a';
	}		/* variables starting w/ non-alphabetic */
	i = 0;
	while (envp[i] != NULL)
	{
		if (!ft_isalpha(envp[i][0]))
			print_export_var(envp[i]);
		i++;
	}
}
