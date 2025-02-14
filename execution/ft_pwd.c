/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_pwd.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aakhrif <aakhrif@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/09 03:32:25 by mlouati           #+#    #+#             */
/*   Updated: 2025/02/11 16:43:58 by aakhrif          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

char	*get_env_value(t_env *my_env, char *var)
{
	t_env	*temp;

	temp = my_env;
	while (temp)
	{
		if (!ft_strcmp(temp->var_name, var))
			return (temp->var_value);
		temp = temp->next;
	}
	return (NULL);
}

int	ft_pwd(t_exec *executor)
{
	char	pwd[4069];
	char	*str;

	str = getcwd(pwd, 4096);
	if (str)
	{
		write(1, str, ft_strlen(str));
		write(1, "\n", 1);
	}
	if (!str)
	{
		write(1, executor->last_pwd, ft_strlen(executor->last_pwd));
		write(1, "\n", 1);
	}
	return (0);
}
