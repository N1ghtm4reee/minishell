/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_quotes.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aakhrif <aakhrif@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/07 18:14:28 by aakhrif           #+#    #+#             */
/*   Updated: 2025/02/13 14:46:57 by aakhrif          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	init_array(t_quote_pos *arr, int size)
{
	int	i;

	i = 0;
	while (i < size)
	{
		arr[i].start = -2;
		arr[i].end = -2;
		i++;
	}
}

int	process_quote(char *s, int i, t_quote_data *data)
{
	if (data->quote_char == '\'')
		data->inside = 0;
	else if (data->quote_char == '"')
		data->inside = 1;
	i++;
	while (s[i] && s[i] != data->quote_char)
	{
		if (s[i] == '$' && data->inside == 1)
			data->should_expand = 1;
		i++;
	}
	return (i);
}

void	handle_quote_case(char *s, t_quote_pos *arr, int *i, int *j)
{
	t_quote_data	data;
	int				start;
	int				end;

	data.quote_char = s[*i];
	data.inside = 0;
	data.should_expand = 0;
	start = *i;
	*i = process_quote(s, *i, &data);
	if (s[*i] == data.quote_char)
	{
		end = *i;
		arr[*j].start = start;
		arr[*j].end = end;
		(*j)++;
	}
}

char	*create_final_string(char *s, t_quote_pos *arr)
{
	int		i;
	int		k;
	int		j;
	char	*str;
	char	*s1;

	i = 0;
	j = 0;
	str = ft_strdup("");
	while (arr[j].start != -2)
	{
		s1 = gc_malloc(sizeof(char) * (arr[j].end - arr[j].start + 1));
		i = arr[j].start + 1;
		k = 0;
		while (i < arr[j].end)
			s1[k++] = s[i++];
		s1[k] = '\0';
		str = ft_strjoin1(str, s1);
		j++;
	}
	return (str);
}

char	*expand_quotes(char *s, int type)
{
	t_quote_pos	*arr;
	char		*result;
	int			size;

	(void)type;
	size = ft_strlen(s) + 1;
	arr = gc_malloc(sizeof(t_quote_pos) * size);
	if (!arr)
		return (s);
	init_array(arr, size);
	find_quote_positions(s, arr);
	if (arr[0].start == -2)
		return (s);
	result = create_final_string(s, arr);
	return (result);
}
