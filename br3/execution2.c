/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution2.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mlouati <mlouati@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/09 03:25:30 by mlouati           #+#    #+#             */
/*   Updated: 2025/02/09 03:30:45 by mlouati          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char *build_command_path(char *path, char *command)
{
    char *cmd_path = ft_strjoin1(path, "/");
    return ft_strjoin1(cmd_path, command);
}

void exec_command(char *cmd_path, t_list *cmd, t_env **my_env)
{
    char **env_exec = env_for_execv(*my_env);
    execve(cmd_path, cmd->command, env_exec);
    free(cmd_path);
}

int try_execute_command(char **paths, t_list *cmd, char *command, t_env **my_env)
{
    int i;
    char *cmd_path;

    i =0;
    while (paths[i])
    {
        cmd_path = build_command_path(paths[i], command);
        if (access(cmd_path, F_OK | X_OK) == 0)
        {
            exec_command(cmd_path, cmd, my_env);
            free(cmd_path);
            return 1;
        }
          if (access(cmd_path, F_OK) == 0)
         {
        write(2, command, ft_strlen(command));
        write(2, ": Permission denied", 19);
        write(2, "\n", 1);
        exit(126);
    }
        free(cmd_path);
        i++;
    }
    return 0;
}

void exec_extern_cmd(t_list *cmd, t_env **my_env)
{
    char *command = cmd->command[0];
    char **paths = get_paths(*my_env);
    if (!paths)
    {
        write(2, "Error: PATH not there\n", 22);
        return;
    }
    if (command[0] == '.' || command[0] == '/')
        path_ready(cmd, my_env);
    if (!try_execute_command(paths, cmd, command, my_env))
    {
        write(2, "Error: command not found: ", 26);
        write(2, command, ft_strlen(command));
        write(2, "\n", 1);
        exit(127);
    }
}
