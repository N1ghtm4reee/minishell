/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aakhrif <aakhrif@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/05 17:56:58 by aakhrif           #+#    #+#             */
/*   Updated: 2025/01/11 18:34:07 by aakhrif          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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
	return (count);
}

static char	*ft_free(char **arr, int k)
{
	while (k > 0)
		free(arr[--k]);
	free(arr);
	return (NULL);
}

static char	**ft_split2(char **arr, int i, char const *s, char c)
{
	int	r[2];
	int	j[2];

	j[0] = 0;
	while (s[i])
	{
		r[0] = i;
		while (s[i] && s[i] != c)
		{
			i++;
		}
		r[1] = i;
		arr[j[0]] = gc_malloc(sizeof(char) * ((r[1] - r[0]) + 1));
		if (arr[j[0]] == NULL)
		{
			ft_free(arr, j[0]);
			return (NULL);
		}
		j[1] = 0;
		while (r[0] < r[1])
			arr[j[0]][j[1]++] = s[r[0]++];
		arr[j[0]++][j[1]] = '\0';
		while (s[i] && s[i] == c)
		{
			if (s[i] == '"')
				break;
			i++;
		}
	}
	arr[j[0]] = 0;
	return (arr);
}

char	**ft_split(char const *s, char c)
{
	int		i;
	char	**d;

	i = 0;
	if (!s || !s[i])
		return (NULL);
	d = gc_malloc(sizeof(char *) * (ft_words((char *)s, c) + 1));
	if (!d)
		return (NULL);
	while (s[i] && s[i] == c)
	{
		
		i++;
	}
	if (!s[i])
	{
		free(d);
		return (NULL);
	}
	d = ft_split2(d, i, s, c);
	return (d);
}
