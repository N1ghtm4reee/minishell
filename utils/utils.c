/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aakhrif <aakhrif@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/05 17:59:07 by aakhrif           #+#    #+#             */
/*   Updated: 2025/02/11 16:45:24 by aakhrif          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	ft_strcmp(char *s1, char *s2)
{
	int	i;

	if (!s1 || !s2)
		return (1);
	i = 0;
	while (s1[i] && s2[i] && (s1[i] == s2[i]))
		i++;
	return (s1[i] - s2[i]);
}

char	*ft_substr_malloc(char *s, int start, int len)
{
	char	*sub;
	int		i;

	if (!s)
		return (0);
	if (start > ft_strlen(s))
		return (ft_strdup(""));
	if (len > (ft_strlen(s) - start))
		len = ft_strlen(s) - start;
	sub = (char *)gc_malloc(len + 1);
	i = 0;
	if (!sub)
		return (NULL);
	while (s[i + start] && i < len)
	{
		sub[i] = s[i + start];
		i++;
	}
	sub[i] = '\0';
	return (sub);
}

char	*ft_substr(char *s, int start, int len)
{
	char	*sub;
	int		i;

	if (!s)
		return (0);
	if (start > ft_strlen(s))
		return (ft_strdup(""));
	if (len > (ft_strlen(s) - start))
		len = ft_strlen(s) - start;
	sub = (char *)gc_malloc(len + 1);
	i = 0;
	if (!sub)
		return (NULL);
	while (s[i + start] && i < len)
	{
		sub[i] = s[i + start];
		i++;
	}
	sub[i] = '\0';
	return (sub);
}

int	get_sign(char c)
{
	if (c == '-')
		return (-1);
	return (1);
}

long long	ft_atoi(char *str, int *error)
{
	int			i;
	int			signe;
	long long	res;
	int			start;

	i = 0;
	signe = 1;
	res = 0;
	while (str[i] == 32 || (str[i] >= 9 && str[i] <= 13))
		i++;
	if (str[i] == '+' || str[i] == '-')
		signe = get_sign(str[i++]);
	while (str[i] == '0')
		i++;
	start = 0;
	while (str[i] >= '0' && str[i] <= '9')
	{
		res = res * 10 + (str[i++] - '0');
		start++;
	}
	if (start >= 20 || res * signe > LONG_MAX || res * signe < LONG_MIN)
		*error = 1;
	return (signe * res);
}
