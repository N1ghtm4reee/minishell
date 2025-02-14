/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_echo.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aakhrif <aakhrif@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/09 03:32:05 by mlouati           #+#    #+#             */
/*   Updated: 2025/02/11 16:43:29 by aakhrif          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	valid_opt(char *str)
{
	int	i;

	i = 0;
	if (!str)
		return (0);
	if (str[i] == '-' && str[i + 1] == 'n')
	{
		i++;
		while (str[i] && str[i] == 'n')
			i++;
		if (str[i] == '\0')
			return (1);
		return (0);
	}
	return (0);
}

void	print_arguments(char **argument, int i, int new_line)
{
	while (argument[i])
	{
		write(1, argument[i], ft_strlen(argument[i]));
		if (argument[++i])
			write(1, " ", 1);
	}
	if (new_line)
		write(1, "\n", 1);
}

int	ft_echo(char **argument)
{
	int	i;
	int	new_line;

	new_line = 1;
	i = 1;
	if (!argument[1])
	{
		write(1, "\n", 1);
		return (EXIT_SUCCESS);
	}
	if (valid_opt(argument[i]))
	{
		while (argument[i] && valid_opt(argument[i]))
			i++;
		new_line = 0;
		if (!argument[i])
			return (EXIT_SUCCESS);
	}
	print_arguments(argument, i, new_line);
	return (EXIT_SUCCESS);
}
