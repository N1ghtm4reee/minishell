/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution2.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mlouati <mlouati@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/09 03:25:30 by mlouati           #+#    #+#             */
/*   Updated: 2025/02/13 18:17:17 by mlouati          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

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

int	exec_builtin(t_list *cmd, t_env **env, t_exec *executor)
{
	int	exit;

	exit = 1;
	if (!ft_strcmp(cmd->command[0], "echo"))
		exit = ft_echo(cmd->command);
	else if (!ft_strcmp(cmd->command[0], "pwd"))
		exit = ft_pwd(executor);
	else if (!ft_strcmp(cmd->command[0], "cd"))
		exit = ft_cd(cmd->command, env, executor);
	else if (!ft_strcmp(cmd->command[0], "export"))
		exit = ft_export(cmd->command, env);
	else if (!ft_strcmp(cmd->command[0], "unset"))
		exit = ft_unset(cmd->command, env);
	else if (!ft_strcmp(cmd->command[0], "env"))
		exit = ft_env(cmd->command, env);
	else
		exit = ft_exit(cmd->command);
	return (exit);
}

void	execute_normal_child(t_pipes *pipes, int has_pipe,
		t_exec *executor, t_list *cmd)
{
	signal(SIGQUIT, handle_sigquit_child);
	if (pipes->prev_fd != -1)
	{
		dup2(pipes->prev_fd, STDIN_FILENO);
		close(pipes->prev_fd);
	}
	if (has_pipe && cmd->type == 0)
	{
		dup2(pipes->pipefd[1], STDOUT_FILENO);
		close(pipes->pipefd[1]);
		close(pipes->pipefd[0]);
	}
	if (handle_redirections(cmd) == -1)
		exit(1);
	exec_extern_cmd(cmd, &executor->env);
	exit(EXIT_FAILURE);
}

void	execute_built_in_child(t_pipes *pipes, int has_pipe, t_exec *executor,
		t_list *cmd)
{
	signal(SIGQUIT, handle_sigquit_child);
	if (pipes->prev_fd != -1)
	{
		dup2(pipes->prev_fd, STDIN_FILENO);
		close(pipes->prev_fd);
	}
	if (has_pipe && cmd->type == 0)
	{
		dup2(pipes->pipefd[1], STDOUT_FILENO);
		close(pipes->pipefd[1]);
		close(pipes->pipefd[0]);
	}
	if (handle_redirections(cmd) == -1)
		exit(1);
	exit(exec_builtin(cmd, &executor->env, executor));
	exit(1);
}

void	execute_built_in_parent(t_exec *executor, t_pipes *pipes, int has_pipe,
		pid_t pid)
{
	t_pids	*new;

	signal(SIGINT, SIG_IGN);
	new = new_pid(pid);
	add_back_pid(&executor->pids, new);
	if (pipes->prev_fd != -1)
		close(pipes->prev_fd);
	if (has_pipe)
	{
		close(pipes->pipefd[1]);
		pipes->prev_fd = pipes->pipefd[0];
	}
}
