/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_export_utils2.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mlouati <mlouati@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/10 02:06:47 by aakhrif           #+#    #+#             */
/*   Updated: 2025/02/12 16:38:06 by mlouati          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

bool	var_exist(t_env *my_env, char *str)
{
	t_env	*temp;

	temp = my_env;
	while (temp)
	{
		if (!ft_strcmp(temp->var_name, str))
			return (true);
		temp = temp->next;
	}
	return (false);
}

bool	is_valid(char *str)
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
		{
			if (str[i] == '+' && (str[i + 1] && str[i + 1] == '='))
				return (true);
			return (false);
		}
		i++;
	}
	return (true);
}

void	ft_export_alone(t_env **my_env)
{
	char	**env_array;
	int		i;

	i = 0;
	env_array = convert_env(*my_env);
	sort_array(env_array);
	while (env_array[i])
	{
		write(1, "declare -x ", 11);
		write(1, env_array[i], ft_strlen(env_array[i]));
		write(1, "\n", 1);
		i++;
	}
}

void	sort_array(char **env_array)
{
	char	*temp;
	int		i;
	int		j;

	i = 0;
	while (env_array[i])
	{
		j = i + 1;
		while (env_array[j])
		{
			if (ft_strcmp(env_array[i], env_array[j]) > 0)
			{
				temp = env_array[i];
				env_array[i] = env_array[j];
				env_array[j] = temp;
			}
			j++;
		}
		i++;
	}
}

char	**convert_env(t_env *env)
{
	int		i;
	char	**env_array;
	t_env	*temp;

	env_array = gc_malloc(sizeof(char *) * (env_size(env) + 1));
	if (!env_array)
		return (NULL);
	i = 0;
	temp = env;
	while (temp)
	{
		if (temp->has_eq_ind)
		{
			env_array[i] = ft_strjoin1(temp->var_name, "=");
			env_array[i] = ft_strjoin1(env_array[i], "\"");
			env_array[i] = ft_strjoin1(env_array[i], temp->var_value);
			env_array[i] = ft_strjoin1(env_array[i], "\"");
		}
		else
			env_array[i] = temp->var_name;
		temp = temp->next;
		i++;
	}
	env_array[i] = NULL;
	return (env_array);
}
