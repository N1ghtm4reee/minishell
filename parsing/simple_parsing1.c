/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   simple_parsing1.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aakhrif <aakhrif@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/10 02:02:42 by aakhrif           #+#    #+#             */
/*   Updated: 2025/02/11 16:44:50 by aakhrif          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	ft_var_compare(char *str, char *var_name)
{
	int	i;

	i = 0;
	if (ft_strlen(str) != ft_strlen(var_name))
		return (1);
	while ((str[i] && var_name[i]) && str[i] == var_name[i])
		i++;
	return (str[i] - var_name[i]);
}

char	*find_in_env(t_env *env, char *str)
{
	t_env	*p;

	p = env;
	while (p)
	{
		if (ft_var_compare(str, p->var_name) == 0)
			return (p->var_value);
		p = p->next;
	}
	return (NULL);
}

int	is_in_arr(int counter, int *arr)
{
	int	i;

	i = 0;
	while (arr[i] != -2)
	{
		if (arr[i] == counter)
			return (1);
		i++;
	}
	return (0);
}

int	new_str_check(char *s, int to_skip)
{
	if (s[to_skip] && !s[to_skip + 1])
		return (1);
	if (!s[0])
		return (1);
	return (0);
}

char	*new_str(char *s, int to_skip)
{
	char	*str;
	int		arr[2];

	if (!s)
		return (NULL);
	if (to_skip >= ft_strlen(s))
		return (ft_strdup(s));
	if (new_str_check(s, to_skip))
		return (ft_strdup(""));
	str = gc_malloc(sizeof(char) * ft_strlen(s));
	arr[0] = 0;
	arr[1] = 0;
	while (s[arr[0]] && arr[0] < ft_strlen(s))
	{
		if (arr[0] == to_skip && arr[0] + 1 < ft_strlen(s))
		{
			arr[0] += 2;
			if (arr[0] >= ft_strlen(s))
				break ;
		}
		if (arr[0] < ft_strlen(s))
			str[arr[1]++] = s[arr[0]++];
	}
	str[arr[1]] = '\0';
	return (str);
}
