/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aakhrif <aakhrif@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/12 18:19:59 by aakhrif           #+#    #+#             */
/*   Updated: 2025/01/14 16:37:37 by aakhrif          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// bool built_in_cmd(t_list *cmd)
// {
//     if(!ft_strcmp(cmd->command[0], "echo"))
//             return (true);
//     else if(!ft_strcmp(cmd->command[0], "pwd"))
//             return (true);
//     else if(!ft_strcmp(cmd->command[0], "cd"))
//             return (true);
//     else if(!ft_strcmp(cmd->command[0], "export"))
//             return (true);
//     else if(!ft_strcmp(cmd->command[0], "unset"))
//             return (true);
//     else if(!ft_strcmp(cmd->command[0], "env"))
//             return (true);
//     else if(!ft_strcmp(cmd->command[0], "exit"))
//             return (true);
//     else
//         return(false);
// }

// void exec_builtin(t_list *cmd, t_env **env)
// {
    
//     if(!ft_strcmp(cmd->command[0], "echo"))
//             ft_echo(cmd->command);
//     else if(!ft_strcmp(cmd->command[0], "pwd"))
//             ft_pwd(env);
//     else if(!ft_strcmp(cmd->command[0], "cd"))
//             ft_cd(cmd->command,env);
//     else if(!ft_strcmp(cmd->command[0], "export"))
//             ft_export(cmd->command, env);
//     else if(!ft_strcmp(cmd->command[0], "unset"))
//             ft_unset(cmd->command, env);
//     else if(!ft_strcmp(cmd->command[0], "env"))
//             ft_env(cmd->command, env);
//     else 
//             ft_exit(cmd->command);
// }
char **get_paths(t_env *my_env)
{
    char *str;
    str = get_env_value(my_env, "PATH");
    if(!str)
        return(NULL);
    return(ft_split(str, ':'));
}
char **env_for_execv(t_env *env)
{
    int     size;
    char    **array;
    t_env   *temp;
    int     i;

    size = env_size(env);
    array = malloc(sizeof(char *) *(size + 1));
    i = 0;
    temp = env;
    while(temp)
    {
        array[i] = ft_strjoin1(temp->var_name, "=");
        array[i] = ft_strjoin1(array[i], temp->var_value);
        temp = temp->next;
        i++;
    }
    array[i] = NULL;
    return(array);
}
void path_ready(t_list *cmd, t_env **my_env)
{
    int found = 0;
    pid_t pid;

    if(access(cmd->command[0], X_OK) == 0)
    {
        found = 1;
        pid = fork();
        if(pid == 0)
        {
            char **env_exec = env_for_execv(*my_env);
            execve(cmd->command[0], cmd->command, env_exec);
            exit(EXIT_FAILURE);

        }
        else if(pid > 0)
        {
            int status;
            waitpid(pid, &status, 0);
        }
        else 
            perror("fork failed\n");
    }
    if(!found)
    {
        write(2, "Error : command not found: ", 27);
        write(2, cmd->command[0], ft_strlen(cmd->command[0]));
        write(2, "\n", 1);
    }
}

void exec_extern_cmd(t_list *cmd, t_env **my_env, t_exec *executor)
{
    char *command;
    pid_t pid;

    pid = fork();
    if (pid < 0)
        perror(" fork failed\n");
    if (!pid)
    {
        if (executor->cmd_size > 1)
        {
            if (cmd->next && cmd->next->type == 1)//pipe | need to handle all specials "red"
            {
                close(executor->exec->pipefd[0]);
                dup2(executor->exec->pipefd[1], STDOUT_FILENO);
            }
            if (cmd->next && cmd->next->type == 3)
            {
                executor->exec->out_fd = open(cmd->next->next->command[0], O_CREAT | O_TRUNC, 0644);
                // close(executor->exec->pipefd[0]);
                // dup2(executor->exec->pipefd[1], STDOUT_FILENO);
            }
            if (executor->exec->prev_type == 3)
            {
                dup2(executor->exec->in_fd, STDOUT_FILENO);
            }
            if (cmd->next && executor->exec->prev_type == 1)
            {
                printf("123\n");
                close(executor->exec->pipefd[1]);
                dup2(executor->exec->pipefd[0], STDIN_FILENO);
            }
            if (executor->cmd_size > 1 && !cmd->next)
            {
                int fd = open("output.txt", O_CREAT | O_TRUNC | O_WRONLY, 0644);
                if (fd == -1)
                    printf("fd\n");
                dup2(STDOUT_FILENO, STDOUT_FILENO);
            }
        }
        cmd->path = get_command(cmd->command[0], executor->path);
        char **env_exec = env_for_execv(*my_env);
        execve(cmd->path, cmd->command, env_exec);
        perror("execve");
        exit(EXIT_FAILURE);//should exit?
    }
    else
        waitpid(pid, NULL, 0);
}
