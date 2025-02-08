/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   which_to_expand.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aakhrif <aakhrif@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/07 00:35:06 by aakhrif           #+#    #+#             */
/*   Updated: 2025/02/08 16:47:00 by aakhrif          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// int *which_to_expand(char *s, int type)
// {
//     if (type == 55 | type == 5)
//         return NULL;
//     int found = 0;
//     int i = 0;
//     char c = -2;
//     int inside = 0;
//     int start;
//     int end;
//     int should_expand = 0;
//     int *arr = gc_malloc(sizeof(int) * 1024);
//     while(i < 1024)
//         arr[i++] = -2;
//     int j = 0;
//     int counter = 0;
//     i = 0;
//     while (s[i])
//     {
//         found = 0;
//         if (s[i] && s[i + 1] && s[i] == '<' && s[i + 1] == '<')
//         {
//             while(s[i] && s[i] == ' ')
//                 i++;
//             while(s[i] && s[i] != ' ')
//                 i++;
//         }
//         else if (s[i] == '\'' || s[i] == '"')
//         {
//             c = s[i];
//             if (c == '\'')
//                 inside = 0;
//             else if (c == '"')
//                 inside = 1;
//             i++;
//             while (s[i] && s[i] != c)
//             {
//                 if (s[i] == '$')
//                     counter++;
//                 if (s[i] == '$' && inside == 1)
//                 {
//                     if (s[i + 1] && ft_isalnum(s[i + 1]))
//                         arr[j++] = counter;
//                     else if (s[i + 1] && s[i + 1] == '$' || s[i + 1] == '_')
//                     {
//                         arr[j++] = counter++;
//                         i++;
//                     }
//                 }
//                 i++;
//             }
//         }
//         else
//         {
//             inside = 1;
//             if (i > 0)
//             {
//                 if (s[i - 1] && (s[i - 1] == '\'' || s[i - 1] == '"'))
//                     start = i - 1;
//             }
//             while(s[i] && (s[i] != '\'' && s[i] != '"'))
//             {
//                 if (s[i] == '$')
//                     counter++;
//                 if (s[i] == '$' && inside == 1)
//                 {
//                     if (s[i + 1] && ft_isalnum(s[i + 1]))
//                         arr[j++] = counter;
//                     else if (s[i + 1] == '$' || s[i + 1] == '_')
//                     {
//                         arr[j++] = counter++;
//                         i++;
//                     }
//                 }
//                 i++;
//             }
//             found = 1;
//         }
//         if (!found)
//             i++;
//     }
//     return arr;
// }

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
			else if (s[var->i + 1] && (s[var->i + 1] == '$' || s[var->i + 1] == '_'))
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
