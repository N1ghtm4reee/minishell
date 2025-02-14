/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_cd.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aakhrif <aakhrif@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/09 22:10:18 by mlouati           #+#    #+#             */
/*   Updated: 2025/02/13 13:54:32 by aakhrif          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	change_directory(char *dir)
{
	if (!dir)
		return (write(2, "cd: HOME not set\n", 17), 1);
	if (chdir(dir) == -1)
	{
		perror("failed to change to the directory\n");
		return (1);
	}
	return (0);
}

int	cd_validate_and_change(char **argument, t_env **my_env)
{
	int	ac;

	ac = 0;
	while (argument[ac])
		ac++;
	if (ac == 1 || (ac == 2 && !ft_strcmp(argument[1], "~")))
		return (change_directory(get_env_value(*my_env, "HOME")));
	if (ac > 2)
	{
		write(2, "bash : cd : too many arguments\n", 31);
		return (1);
	}
	else
		return (change_directory(argument[1]));
}

void	update_old_pwd(t_env **my_env, char *old_pwd, char *var, t_env *new)
{
	if (var_exist(*my_env, "OLDPWD"))
		change_var_value(my_env, "OLDPWD", old_pwd);
	else
	{
		var = ft_strjoin1("OLDPWD", "=");
		var = ft_strjoin1(var, old_pwd);
		new = new_env(var);
		if (new)
			add_back_env(my_env, new);
	}
}

void	update_pwd_variables(t_env **my_env, char *old_pwd, char *new_pwd)
{
	char	*var;
	t_env	*new;

	var = NULL;
	new = NULL;
	if (old_pwd)
		update_old_pwd(my_env, old_pwd, var, new);
	if (new_pwd)
	{
		if (var_exist(*my_env, "PWD"))
			change_var_value(my_env, "PWD", new_pwd);
		else
		{
			var = ft_strjoin1("PWD", "=");
			var = ft_strjoin1(var, new_pwd);
			new = new_env(var);
			if (new)
				add_back_env(my_env, new);
		}
	}
}

int	ft_cd(char **argument, t_env **my_env, t_exec *executor)
{
	char	old_buff[4096];
	char	new_buff[4096];
	char	*old_pwd;
	char	*new_pwd;

	old_pwd = getcwd(old_buff, 4096);
	if (cd_validate_and_change(argument, my_env))
		return (1);
	new_pwd = getcwd(new_buff, 4096);
	if (new_pwd)
	{
		executor->last_pwd = ft_strdup_malloc(new_pwd);
		update_pwd_variables(my_env, old_pwd, new_pwd);
	}
	else
		write(2, "failed to change to the directory\n", 35);
	return (0);
}
