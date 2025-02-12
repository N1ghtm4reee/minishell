/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_env.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aakhrif <aakhrif@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/09 03:31:34 by mlouati           #+#    #+#             */
/*   Updated: 2025/02/11 16:43:32 by aakhrif          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

t_env	*new_env(char *str)
{
	t_env	*new;
	int		indice;

	indice = equal_signe(str);
	new = (t_env *)gc_malloc(sizeof(t_env));
	if (!new)
		return (NULL);
	new->var_name = ft_substr_malloc(str, 0, indice);
	if (!new->var_name)
		return (NULL);
	new->var_value = ft_substr_malloc(str, indice + 1, ft_strlen(str) - indice
			- 1);
	if (!new->var_value)
		return (NULL);
	new->has_eq_ind = equal_flag(str);
	new->next = NULL;
	return (new);
}

void	add_back_env(t_env **my_env, t_env *new)
{
	t_env	*temp;

	if (!new || !my_env)
		return ;
	if (!(*my_env))
		*(my_env) = new;
	else
	{
		temp = *my_env;
		while (temp->next)
			temp = temp->next;
		temp->next = new;
	}
}

void	print_env(t_env *my_env)
{
	t_env	*temp;

	temp = my_env;
	while (temp)
	{
		if (temp->has_eq_ind)
		{
			write(1, temp->var_name, ft_strlen(temp->var_name));
			write(1, "=", 1);
			write(1, temp->var_value, ft_strlen(temp->var_value));
			write(1, "\n", 1);
		}
		temp = temp->next;
	}
}

int	ft_env(char **argument, t_env **env)
{
	int	ac;

	ac = 0;
	while (argument[ac])
		ac++;
	if (ac == 1)
		print_env(*env);
	return (0);
}
