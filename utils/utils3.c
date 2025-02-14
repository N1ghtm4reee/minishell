/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils3.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aakhrif <aakhrif@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/12 16:39:50 by mlouati           #+#    #+#             */
/*   Updated: 2025/02/13 14:45:30 by aakhrif          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	get_sign(char c)
{
	if (c == '-')
		return (-1);
	return (1);
}

int	check_error_pos(int start, long long res, char *str, int i)
{
	if (start >= 20 || res > LONG_MAX || (start == 19 && str[i - 1] > '7'))
		return (1);
	return (0);
}

int	check_error_neg(int start, long long res, char *str, int i)
{
	if (start >= 20 || res *(-1) < LONG_MIN
		|| (start == 19 && str[i -1] == '9'))
		return (1);
	return (0);
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
	if (signe == 1)
		*error = check_error_pos(start, res, str, i);
	if (signe == -1)
		*error = check_error_neg(start, res, str, i);
	return (signe * res);
}

t_list	*get_redir(t_list *cmd, int *f)
{
	t_list	*redir;

	redir = cmd->next;
	if (redir && redir->type == 1)
	{
		if (redir->next && redir->next->type != 0)
		{
			*f = 1;
			redir = redir->next;
		}
	}
	return (redir);
}
