/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_exit.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mlouati <mlouati@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/09 03:31:40 by mlouati           #+#    #+#             */
/*   Updated: 2025/02/13 18:18:14 by mlouati          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	ft_is_number(char *str)
{
	int	i;

	i = 0;
	if (str[i] == '+' || str[i] == '-')
		i++;
	if (str[i] == '\0')
		return (0);
	while (str[i])
	{
		if (str[i] < '0' || str[i] > '9')
			return (0);
		i++;
	}
	return (1);
}

int	ft_exit(char **argument)
{
	int			error;
	long long	num;

	error = 0;
	if (!argument[1])
		exit(get_exit_status());
	if (argument[1] && !ft_is_number(argument[1]))
		return (ft_printf(2, "bash : exit: "),
			ft_printf(2, argument[1]),
			ft_printf(2, ": numeric argument required\n"),
			set_exit_status(2), exit(2), 2);
	num = ft_atoi(argument[1], &error);
	if (error)
		return (ft_printf(2, "bash : exit: "),
			ft_printf(2, argument[1]),
			ft_printf(2, ": numeric argument required\n"),
			set_exit_status(2), exit(2), 2);
	if (argument[2])
	{
		ft_printf(2, "bash: exit: too many arguments\n");
		return (1);
	}
	set_exit_status(num % 256);
	exit(num % 256);
	return (0);
}
