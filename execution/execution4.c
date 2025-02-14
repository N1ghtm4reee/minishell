/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution4.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aakhrif <aakhrif@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/10 01:57:35 by aakhrif           #+#    #+#             */
/*   Updated: 2025/02/12 21:40:13 by aakhrif          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

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

	if (ft_strcmp(cmd->command[0], "..") && ft_strcmp(cmd->command[0], ".")
		&& access(cmd->command[0], F_OK | X_OK) == 0)
	{
		env_exec = env_for_execv(*my_env);
		execve(cmd->command[0], cmd->command, env_exec);
	}
	else if (ft_strcmp(cmd->command[0], "..") && ft_strcmp(cmd->command[0], ".")
		&& access(cmd->command[0], F_OK) == 0)
	{
		write(2, cmd->command[0], ft_strlen(cmd->command[0]));
		write(2, ": Permission denied", 19);
		write(2, "\n", 1);
		exit(126);
	}
	else if (ft_strcmp(cmd->command[0], ".") == 0)
	{
		ft_printf(2, "bash: .: filename argument required\n");
		ft_printf(2, ".: usage: . filename [arguments]\n");
		exit(2);
	}
	else
		return (cmd_not_found(cmd->command[0]), exit(127));
}

void	look_for_path(int *found, char *path, t_env **my_env, t_list *cmd)
{
	char	**env_exec;
	char	*cmd_path;
	char	*command;

	command = cmd->command[0];
	cmd_path = NULL;
	cmd_path = ft_strjoin1(path, "/");
	cmd_path = ft_strjoin1(cmd_path, command);
	if (access(cmd_path, F_OK | X_OK) == 0)
	{
		*found = 1;
		env_exec = env_for_execv(*my_env);
		execve(cmd_path, cmd->command, env_exec);
	}
	else if (access(cmd_path, F_OK) == 0)
	{
		write(2, command, ft_strlen(command));
		write(2, ": Permission denied", 19);
		write(2, "\n", 1);
		exit(126);
	}
}

void	exec_extern_cmd(t_list *cmd, t_env **my_env)
{
	char	**paths;
	char	*command;
	int		i;
	int		found;

	if (!cmd || !cmd->command || !cmd->command[0])
		return ;
	found = 0;
	command = cmd->command[0];
	if (command[0] == '.' || command[0] == '/')
		path_ready(cmd, my_env);
	paths = get_paths(*my_env);
	if (paths)
	{
		i = -1;
		while (paths[++i])
			look_for_path(&found, paths[i], my_env, cmd);
	}
	if (!found)
	{
		write(2, "Error : command not found: ", 27);
		write(2, command, ft_strlen(command));
		write(2, "\n", 1);
		exit(127);
	}
}
