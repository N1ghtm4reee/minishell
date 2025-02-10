/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution2.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aakhrif <aakhrif@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/09 03:25:30 by mlouati           #+#    #+#             */
/*   Updated: 2025/02/10 15:14:39 by aakhrif          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*build_command_path(char *path, char *command)
{
	char	*cmd_path;

	cmd_path = ft_strjoin1(path, "/");
	return (ft_strjoin1(cmd_path, command));
}

void	exec_command(char *cmd_path, t_list *cmd, t_env **my_env)
{
	char	**env_exec;

	env_exec = env_for_execv(*my_env);
	execve(cmd_path, cmd->command, env_exec);
}

int	try_execute_command(char **paths, t_list *cmd, char *command,
		t_env **my_env)
{
	int		i;
	char	*cmd_path;

	if (!paths)
		return (0);
	i = 0;
	while (paths[i])
	{
		cmd_path = build_command_path(paths[i], command);
		if (access(cmd_path, F_OK | X_OK) == 0)
		{
			exec_command(cmd_path, cmd, my_env);
			return (1);
		}
		if (access(cmd_path, F_OK) == 0)
		{
			write(2, command, ft_strlen(command));
			write(2, ": Permission denied", 19);
			write(2, "\n", 1);
			exit(126);
		}
		i++;
	}
	return (0);
}

void	exec_extern_cmd(t_list *cmd, t_env **my_env)
{
	char	*command;
	char	**paths;

	if (!cmd || !cmd->command)
		return ;
	command = cmd->command[0];
	if (command[0] == '.' || command[0] == '/')
		path_ready(cmd, my_env);
	paths = get_paths(*my_env);
	if (!try_execute_command(paths, cmd, command, my_env))
	{
		write(2, "Error: command not found: ", 26);
		write(2, command, ft_strlen(command));
		write(2, "\n", 1);
		set_exit_status(127);
		exit(127);
	}
}
