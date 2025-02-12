/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   which_to_expand.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aakhrif <aakhrif@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/07 00:35:06 by aakhrif           #+#    #+#             */
/*   Updated: 2025/02/11 16:45:02 by aakhrif          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	*initialize_array(int size)
{
	int	*arr;
	int	i;

	arr = gc_malloc(sizeof(int) * size);
	i = 0;
	while (i < size)
		arr[i++] = -2;
	return (arr);
}

void	ft_ft_handle_quotes(char *s, int *arr, t_wte *var)
{
	var->inside = 1;
	while (s[var->i] && (s[var->i] != '\'' && s[var->i] != '"'))
	{
		if (s[var->i] == '$')
			(var->counter)++;
		if (s[var->i] == '$' && var->inside == 1)
		{
			if (s[var->i + 1] && ft_isalnum(s[var->i + 1]))
				arr[(var->j)++] = var->counter;
			else if (s[var->i + 1]
				&& (s[var->i + 1] == '$' || s[var->i + 1] == '_'))
			{
				arr[(var->j)++] = (var->counter)++;
				(var->i)++;
			}
		}
		(var->i)++;
	}
}

void	handle_non_quotes(char *s, int *arr, t_wte *var)
{
	var->inside = 1;
	while (s[var->i] && (s[var->i] != '\'' && s[var->i] != '"'))
	{
		if (s[var->i] == '$')
			(var->counter)++;
		if (s[var->i] == '$' && var->inside == 1)
		{
			if (s[var->i + 1] && ft_isalnum(s[var->i + 1]))
				arr[(var->j)++] = var->counter;
			else if (s[var->i + 1] == '$' || s[var->i + 1] == '_')
			{
				arr[(var->j)++] = (var->counter)++;
				(var->i)++;
			}
		}
		(var->i)++;
	}
}

int	*which_to_expand(char *s, int type)
{
	int		*arr;
	t_wte	var;

	if (type == 55 || type == 5)
		return (NULL);
	arr = initialize_array(ft_strlen(s) + 1);
	var.counter = 0;
	var.i = 0;
	var.inside = 0;
	var.j = 0;
	while (s[var.i])
	{
		if (s[var.i] == '\'' || s[var.i] == '"')
			ft_ft_handle_quotes(s, arr, &var);
		else
			handle_non_quotes(s, arr, &var);
		if (!s[var.i])
			break ;
		var.i++;
	}
	return (arr);
}
