/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution2.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aakhrif <aakhrif@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/09 03:25:30 by mlouati           #+#    #+#             */
/*   Updated: 2025/02/10 17:53:01 by aakhrif          ###   ########.fr       */
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

void	execute_normal_child(t_pipes *pipes, int has_pipe, t_exec *executor,
		t_list *cmd)
{
	signal(SIGQUIT, handle_sigquit_child);
	if (pipes->prev_fd != -1)
	{
		dup2(pipes->prev_fd, STDIN_FILENO);
		close(pipes->prev_fd);
	}
	if (has_pipe)
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
	signal(SIGINT, handle_ctrlc);
	if (pipes->prev_fd != -1)
	{
		dup2(pipes->prev_fd, STDIN_FILENO);
		close(pipes->prev_fd);
	}
	if (has_pipe)
	{
		dup2(pipes->pipefd[1], STDOUT_FILENO);
		close(pipes->pipefd[1]);
		close(pipes->pipefd[0]);
	}
	if (handle_redirections(cmd) == -1)
		exit(1);
	exec_builtin(cmd, &executor->env, executor);
	exit(EXIT_SUCCESS);
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
