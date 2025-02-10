/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution3.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aakhrif <aakhrif@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/09 21:05:05 by mlouati           #+#    #+#             */
/*   Updated: 2025/02/10 14:17:15 by aakhrif          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	handle_child_process_non_builtin(t_list *cmd, t_exec *executor,
		t_pipes *pipe, int *has_pipe)
{
	if (!cmd || !cmd->command || !cmd->command[0])
		return ;
	signal(SIGQUIT, handle_sigquit_child);
	if (pipe->prev_fd != -1)
	{
		dup2(pipe->prev_fd, STDIN_FILENO);
		close(pipe->prev_fd);
	}
	if (*has_pipe)
	{
		dup2(pipe->pipefd[1], STDOUT_FILENO);
		close(pipe->pipefd[1]);
		close(pipe->pipefd[0]);
	}
	if (handle_redirections(cmd) == -1)
		exit(1);
	if (!cmd->type)
		exec_extern_cmd(cmd, &executor->env);
		exit(EXIT_FAILURE);
}

void	exec_non_bultins(t_pipes *pipe, int *has_pipe, t_exec *executor,
		t_list *cmd)
{
	pid_t	pid;

	pid = fork();
	if (pid == 0)
		handle_child_process_non_builtin(cmd, executor, pipe, has_pipe);
	else if (pid > 0)
		handle_parent_process(pid, executor, pipe, has_pipe);
	else
		perror("fork failed");
}

void	exec_builtin_without_pipe(t_list *cmd, t_exec *executor)
{
	if (handle_redirections(cmd) == -1)
		exit(1);
	exec_builtin(cmd, &executor->env, executor);
}

void	handle_child_process_builtin(t_list *cmd, t_exec *executor,
		t_pipes *pipe, int *has_pipe)
{
	signal(SIGQUIT, handle_sigquit_child);
	signal(SIGINT, handle_ctrlc);
	if (pipe->prev_fd != -1)
	{
		dup2(pipe->prev_fd, STDIN_FILENO);
		close(pipe->prev_fd);
	}
	if (*has_pipe)
	{
		dup2(pipe->pipefd[1], STDOUT_FILENO);
		close(pipe->pipefd[1]);
		close(pipe->pipefd[0]);
	}
	if (handle_redirections(cmd) == -1)
		exit(1);
	exec_builtin(cmd, &executor->env, executor);
	exit(EXIT_SUCCESS);
}

void	exec_builtin_with_pipe(t_pipes *pipe, int *has_pipe, t_list *cmd,
		t_exec *executor)
{
	pid_t	pid;

	pid = fork();
	if (pid == 0)
		handle_child_process_builtin(cmd, executor, pipe, has_pipe);
	else if (pid > 0)
		handle_parent_process(pid, executor, pipe, has_pipe);
	else
		perror("fork failed");
}
