/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_export.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aakhrif <aakhrif@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/10 00:54:29 by aakhrif           #+#    #+#             */
/*   Updated: 2025/02/12 21:49:24 by aakhrif          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	change_var_value_export(t_env **my_env, char *str_name,
		char *str_value, char *var)
{
	t_env	*temp;

	temp = *my_env;
	while (temp)
	{
		if (!ft_strcmp(temp->var_name, str_name))
		{
			temp->has_eq_ind = equal_flag(var);
			temp->var_value = ft_strdup(str_value);
			break ;
		}
		temp = temp->next;
	}
}

void	change_var_value(t_env **my_env, char *str_name, char *str_value)
{
	t_env	*temp;

	temp = *my_env;
	while (temp)
	{
		if (!ft_strcmp(temp->var_name, str_name))
		{
			temp->var_value = ft_strdup(str_value);
			break ;
		}
		temp = temp->next;
	}
}

void	export_to_env(t_env **my_env, char *var)
{
	int		indice;
	char	*str_name;
	char	*str_value;
	t_env	*new;

	indice = equal_signe(var);
	str_name = ft_substr(var, 0, indice);
	indice++;
	while (indice < ft_strlen(var) && var[indice] == ' ')
		indice++;
	str_value = ft_substr(var, indice, ft_strlen(var) - indice);
	if (!str_name)
		return ;
	if (var_exist(*my_env, str_name))
		change_var_value_export(my_env, str_name, str_value, var);
	else
	{
		new = new_env(var);
		if (new)
			add_back_env(my_env, new);
	}
}

void	export_error(char *argument)
{
	ft_printf(2, "minishell: export: '");
	ft_printf(2, argument);
	ft_printf(2, "': not a valid identifier\n");
}

int	ft_export(char **argument, t_env **my_env)
{
	int	ac;
	int	i;

	ac = 0;
	while (argument[ac])
		ac++;
	if (ac == 1)
		ft_export_alone(my_env);
	if (ac > 1)
	{
		i = 0;
		while (argument[++i])
		{
			if (is_valid(argument[i]))
			{
				if (is_special_case(argument[i]))
					export_special_case(my_env, argument[i]);
				else
					export_to_env(my_env, argument[i]);
			}
			else
				return (export_error(argument[i]), 1);
		}
	}
	return (0);
}
