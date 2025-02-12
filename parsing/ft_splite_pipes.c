/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_splite_pipes.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aakhrif <aakhrif@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/06 20:48:40 by aakhrif           #+#    #+#             */
/*   Updated: 2025/02/11 16:44:40 by aakhrif          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	skip_spaces(char *s, int *i)
{
	while (s[*i] && ft_isspace(s[*i]))
		(*i)++;
}

void	handle_non_operators(char *s, int *i, char **arr, int *k)
{
	int		st;
	char	c;

	st = *i;
	while (s[*i] && s[*i] != '|' && s[*i] != '<' && s[*i] != '>')
	{
		if (s[*i] == '\"' || s[*i] == '\'')
		{
			c = s[*i];
			(*i)++;
			while (s[*i] && s[*i] != c)
				(*i)++;
			if (!s[*i])
			{
				(*i)--;
				break ;
			}
		}
		(*i)++;
	}
	if (*i > st)
		arr[(*k)++] = ft_substr(s, st, *i - st);
}

void	handle_double_operators(char *s, int *i, char **arr, int *k)
{
	if (s[*i] && s[*i + 1])
	{
		if (s[*i] == '>' && s[*i + 1] == '>')
		{
			arr[(*k)++] = ft_strdup(">>");
			*i += 2;
		}
		else if (s[*i] == '<' && s[*i + 1] == '<')
		{
			arr[(*k)++] = ft_strdup("<<");
			*i += 2;
		}
	}
}

void	handle_single_operators(char *s, int *i, char **arr, int *k)
{
	if (s[*i])
	{
		if (s[*i] == '|')
			arr[(*k)++] = ft_strdup("|");
		else if (s[*i] == '<')
			arr[(*k)++] = ft_strdup("<");
		else if (s[*i] == '>')
			arr[(*k)++] = ft_strdup(">");
		(*i)++;
	}
}

char	**ft_split_pipes(char *s)
{
	char	**arr;
	int		i;
	int		k;

	i = 0;
	k = 0;
	arr = gc_malloc(sizeof(char *) * (ft_strlen(s) + 1));
	if (!arr)
		return (NULL);
	while (s[i])
	{
		skip_spaces(s, &i);
		if (!s[i])
			break ;
		if (s[i] != '|' && s[i] != '<' && s[i] != '>')
			handle_non_operators(s, &i, arr, &k);
		else if ((s[i] == '>' && s[i + 1] == '>') || (s[i] == '<'
				&& s[i + 1] == '<'))
			handle_double_operators(s, &i, arr, &k);
		else
			handle_single_operators(s, &i, arr, &k);
		skip_spaces(s, &i);
	}
	arr[k] = NULL;
	return (arr);
}
