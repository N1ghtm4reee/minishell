/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aakhrif <aakhrif@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/05 17:56:58 by aakhrif           #+#    #+#             */
/*   Updated: 2025/02/11 16:45:13 by aakhrif          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static int	ft_words(char *s, char c)
{
	int	i;
	int	count;

	i = 0;
	count = 0;
	while (s[i])
	{
		while (s[i] && s[i] == c)
			i++;
		if (s[i])
			count++;
		while (s[i] && s[i] != c)
			i++;
	}
	if (!count)
		count++;
	return (count);
}

static char	**allocate_and_copy_substring(char **arr, int *j,
		t_split_info *info)
{
	int	k;

	arr[*j] = gc_malloc(sizeof(char) * ((info->end - info->start) + 1));
	if (arr[*j] == NULL)
		return (NULL);
	k = 0;
	while (info->start < info->end)
		arr[*j][k++] = info->str[info->start++];
	arr[(*j)++][k] = '\0';
	return (arr);
}

static void	skip_delimiters(char const *s, int *i, char c)
{
	while (s[*i] && s[*i] == c)
	{
		if (s[*i] == '"')
			break ;
		(*i)++;
	}
}

static char	**ft_split2(char **arr, int i, char const *s, char c)
{
	int				r[2];
	int				j[2];
	t_split_info	info;

	j[0] = 0;
	j[1] = 0;
	info.str = s;
	if (!s[i])
		arr[j[0]++] = ft_strdup(s);
	else
	{
		while (s[i])
		{
			r[0] = i;
			while (s[i] && s[i] != c)
				i++;
			r[1] = i;
			info.start = r[0];
			info.end = r[1];
			if (!allocate_and_copy_substring(arr, &j[0], &info))
				return (NULL);
			skip_delimiters(s, &i, c);
		}
	}
	return (arr[j[0]] = 0, arr);
}

char	**ft_split(char const *s, char c)
{
	int		i;
	char	**d;
	int		count;

	i = 0;
	count = ft_words((char *)s, c);
	while (s[i] && s[i] == c)
		i++;
	if (!s)
		return (NULL);
	i = 0;
	d = gc_malloc(sizeof(char *) * (count + 1));
	if (!d)
		return (NULL);
	while (s[i] && s[i] == c)
		i++;
	d = ft_split2(d, i, s, c);
	return (d);
}
