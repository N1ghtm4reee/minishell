/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split_custom.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aakhrif <aakhrif@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/12 09:48:47 by aakhrif           #+#    #+#             */
/*   Updated: 2025/02/12 11:02:33 by aakhrif          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static int	count_spaces(char *str, int i)
{
	int	count;

	count = 0;
	if (!str)
		return (-1);
	while (str[i] && str[i] == ' ')
	{
		count++;
		i++;
	}
	return (count);
}

static int	word_length(char *str)
{
	int	len;

	len = 0;
	while (str[len] && str[len] != ' ')
		len++;
	return (len);
}

static char	*make_word(char *str, int len)
{
	char	*word;
	int		i;

	i = 0;
	word = gc_malloc(sizeof(char) * (len + 1));
	if (!word)
		return (NULL);
	while (i < len)
	{
		word[i] = str[i];
		i++;
	}
	word[i] = '\0';
	return (word);
}

static char	*make_spaces(int count)
{
	char	*spaces;
	int		i;

	i = 0;
	count -= 2;
	spaces = gc_malloc(sizeof(char) * (count + 1));
	if (!spaces)
		return (NULL);
	while (i < count)
	{
		spaces[i] = ' ';
		i++;
	}
	spaces[i] = '\0';
	return (spaces);
}

char	**ft_split_custom(char *s, char c)
{
	char	**result;
	int		i;
	int		j;

	i = 0;
	j = 0;
	result = gc_malloc(sizeof(char *) * (ft_strlen(s) + 1));
	while (s[i])
	{
		if (s[i] == c)
		{
			result[j] = make_spaces(count_spaces(s, i));
			j++;
			i += count_spaces(s, i);
		}
		else
		{
			result[j] = make_word(&s[i], word_length(&s[i]));
			j++;
			i += word_length(&s[i]);
		}
	}
	result[j] = NULL;
	return (result);
}
