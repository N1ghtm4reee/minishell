/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution1.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mlouati <mlouati@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/09 03:25:23 by mlouati           #+#    #+#             */
/*   Updated: 2025/02/09 22:27:26 by mlouati          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

bool	built_in_cmd(t_list *cmd)
{
	if (!ft_strcmp(cmd->command[0], "echo"))
		return (true);
	else if (!ft_strcmp(cmd->command[0], "pwd"))
		return (true);
	else if (!ft_strcmp(cmd->command[0], "cd"))
		return (true);
	else if (!ft_strcmp(cmd->command[0], "export"))
		return (true);
	else if (!ft_strcmp(cmd->command[0], "unset"))
		return (true);
	else if (!ft_strcmp(cmd->command[0], "env"))
		return (true);
	else if (!ft_strcmp(cmd->command[0], "exit"))
		return (true);
	else
		return (false);
}

void	exec_builtin(t_list *cmd, t_env **env, t_exec *executor)
{
	if (!ft_strcmp(cmd->command[0], "echo"))
		set_exit_status(ft_echo(cmd->command));
	else if (!ft_strcmp(cmd->command[0], "pwd"))
		set_exit_status(ft_pwd(executor));
	else if (!ft_strcmp(cmd->command[0], "cd"))
		set_exit_status(ft_cd(cmd->command, env, executor));
	else if (!ft_strcmp(cmd->command[0], "export"))
		set_exit_status(ft_export(cmd->command, env));
	else if (!ft_strcmp(cmd->command[0], "unset"))
		set_exit_status(ft_unset(cmd->command, env));
	else if (!ft_strcmp(cmd->command[0], "env"))
		set_exit_status(ft_env(cmd->command, env));
	else
		ft_exit(cmd->command);
}

char	**get_paths(t_env *my_env)
{
	char	*str;

	str = get_env_value(my_env, "PATH");
	if (!str)
		return (NULL);
	return (ft_split(str, ':'));
}

char	**env_for_execv(t_env *env)
{
	int		size;
	char	**array;
	t_env	*temp;
	int		i;

	size = env_size(env);
	array = gc_malloc(sizeof(char *) * (size + 1));
	i = 0;
	temp = env;
	while (temp)
	{
		array[i] = ft_strjoin1(temp->var_name, "=");
		array[i] = ft_strjoin1(array[i], temp->var_value);
		temp = temp->next;
		i++;
	}
	array[i] = NULL;
	return (array);
}

void	path_ready(t_list *cmd, t_env **my_env)
{
	char	**env_exec;

	if (access(cmd->command[0], F_OK | X_OK) == 0)
	{
		env_exec = env_for_execv(*my_env);
		execve(cmd->command[0], cmd->command, env_exec);
	}
	else if (access(cmd->command[0], F_OK) == 0)
	{
		write(2, cmd->command[0], ft_strlen(cmd->command[0]));
		write(2, ": Permission denied", 19);
		write(2, "\n", 1);
		exit(126);
	}
	else
	{
		write(2, "Error : command not found: ", 27);
		write(2, cmd->command[0], ft_strlen(cmd->command[0]));
		write(2, "\n", 1);
		exit(127);
	}
}
