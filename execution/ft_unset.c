/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_unset.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aakhrif <aakhrif@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/09 03:32:44 by mlouati           #+#    #+#             */
/*   Updated: 2025/02/11 16:44:01 by aakhrif          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

bool	is_valid_unset(char *str)
{
	int	i;

	i = 0;
	if (!str)
		return (false);
	if (!str[0] || (str[0] != '_' && !ft_isalpha(str[0])))
		return (false);
	while (str[i] && str[i] != '=')
	{
		if (!ft_isalnum(str[i]) && str[i] != '_')
			return (false);
		i++;
	}
	if (str[i] != '\0')
		return (false);
	return (true);
}

void	unset_argument(char *str, t_env **my_env)
{
	t_env	*temp;
	t_env	*prev;

	if (!my_env || !*my_env)
		return ;
	temp = *my_env;
	prev = NULL;
	while (temp)
	{
		if (!ft_strcmp(str, temp->var_name))
		{
			if (prev)
				prev->next = temp->next;
			else
				*my_env = temp->next;
			return ;
		}
		prev = temp;
		temp = temp->next;
	}
}

int	ft_unset(char **argument, t_env **my_env)
{
	int	ac;
	int	i;

	ac = 0;
	while (argument[ac])
		ac++;
	if (ac == 1)
		return (0);
	i = 1;
	while (argument[i])
	{
		if (is_valid_unset(argument[i]))
		{
			if (var_exist(*my_env, argument[i]))
				unset_argument(argument[i], my_env);
		}
		i++;
	}
	return (0);
}
