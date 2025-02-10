/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_exit.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aakhrif <aakhrif@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/09 03:31:40 by mlouati           #+#    #+#             */
/*   Updated: 2025/02/10 21:37:47 by aakhrif          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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

void	ft_exit(char **argument)
{
	int			error;
	long long	num;

	error = 0;
	if (!argument[1])
		exit(get_exit_status());
	if (argument[1] && !ft_is_number(argument[1]))
	{
		printf("bash : exit: %s: numeric argument required\n", argument[1]);
		set_exit_status(2);
		exit(2);
	}
	num = ft_atoi(argument[1], &error);
	if (error)
		return (printf("bash : exit: %s: numeric argument required\n",
				argument[1]), exit(2));
	if (argument[2])
	{
		printf("bash: exit: too many arguments\n");
		set_exit_status(1);
		return ;
	}
	set_exit_status(num % 256);
	exit(num % 256);
}
