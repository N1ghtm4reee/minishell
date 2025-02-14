/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_export_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aakhrif <aakhrif@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/10 01:42:22 by aakhrif           #+#    #+#             */
/*   Updated: 2025/02/11 16:43:48 by aakhrif          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	plus_signe_index(char *str)
{
	int	i;

	i = 0;
	while (str[i] && str[i] != '+')
		i++;
	return (i);
}

void	change_var_value_special(t_env **my_env, char *str_name,
		char *str_value)
{
	t_env	*temp;

	temp = *my_env;
	while (temp)
	{
		if (!ft_strcmp(temp->var_name, str_name))
		{
			temp->var_value = ft_strjoin1(temp->var_value, str_value);
			temp->has_eq_ind = 1;
			break ;
		}
		temp = temp->next;
	}
}

void	export_special_case(t_env **my_env, char *str)
{
	int		indice;
	char	*str_name;
	char	*str_value;
	t_env	*new;

	indice = plus_signe_index(str);
	str_name = ft_substr(str, 0, indice);
	indice = indice + 2;
	while (str[indice] && str[indice] == ' ')
		indice++;
	str_value = ft_substr(str, indice, ft_strlen(str) - indice);
	if (!str_name)
		return ;
	if (var_exist(*my_env, str_name))
		change_var_value_special(my_env, str_name, str_value);
	else
	{
		new = new_env_special(str);
		if (new)
			add_back_env(my_env, new);
	}
}

int	is_special_case(char *str)
{
	int	i;

	i = 0;
	while (str[i] && str[i] != '+' && str[i] != '=')
	{
		i++;
	}
	if (str[i] && str[i] == '+')
		return (1);
	return (0);
}

t_env	*new_env_special(char *str)
{
	t_env	*new;
	int		indice;

	indice = plus_signe_index(str);
	new = (t_env *)gc_malloc(sizeof(t_env));
	if (!new)
		return (NULL);
	new->var_name = ft_substr(str, 0, indice);
	if (!new->var_name)
		return (NULL);
	new->var_value = ft_substr(str, indice + 2, ft_strlen(str) - indice - 2);
	if (!new->var_value)
		return (NULL);
	new->has_eq_ind = 1;
	new->next = NULL;
	return (new);
}
