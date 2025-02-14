/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aakhrif <aakhrif@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/10 18:52:19 by aakhrif           #+#    #+#             */
/*   Updated: 2025/02/13 16:23:06 by aakhrif          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	execute_built_in(t_exec *executor, t_pipes *pipes, int has_pipe,
		t_list *cmd)
{
	pid_t	pid;

	if (cmds_count(executor->commands_list) != 1)
	{
		pid = fork();
		if (pid == 0)
			execute_built_in_child(pipes, has_pipe, executor, cmd);
		else if (pid > 0)
			execute_built_in_parent(executor, pipes, has_pipe, pid);
		else
			perror("fork failed");
	}
	else
	{
		if (handle_redirections(cmd) == -1)
			return ;
		set_exit_status(exec_builtin(cmd, &executor->env, executor));
	}
}

void	execute_normal_cmd(t_exec *executor, t_pipes *pipes, int has_pipe,
		t_list *cmd)
{
	pid_t	pid;
	t_pids	*new;

	signal(SIGQUIT, sig_quit);
	signal(SIGINT, signinthandler);
	pid = fork();
	if (pid == 0)
		execute_normal_child(pipes, has_pipe, executor, cmd);
	else if (pid > 0)
	{
		signal(SIGINT, signinthandler);
		new = new_pid(pid);
		add_back_pid(&executor->pids, new);
		if (pipes->prev_fd != -1)
			close(pipes->prev_fd);
		if (has_pipe && cmd->type == 0)
		{
			close(pipes->pipefd[1]);
			pipes->prev_fd = pipes->pipefd[0];
		}
	}
	else
		perror("fork failed");
}

void	look_for_pipe(int *has_pipe, t_list *cmd)
{
	t_list	*tmp;

	*has_pipe = 0;
	tmp = cmd;
	while (tmp)
	{
		if (tmp->type == 1)
		{
			*has_pipe = 1;
			break ;
		}
		tmp = tmp->next;
	}
}

void	init_execute_cmds(t_pipes *pipes, int *in, int *out)
{
	pipes->pipefd[0] = 0;
	pipes->pipefd[1] = 0;
	pipes->prev_fd2 = -1;
	pipes->prev_fd = -1;
	*in = dup(STDIN_FILENO);
	*out = dup(STDOUT_FILENO);
}

void	exceute_cmds(t_exec *executor)
{
	t_pipes	pipes;
	t_list	*cmd;
	int		has_pipe;
	int		saved_std_in;
	int		saved_std_out;

	cmd = executor->commands_list;
	init_execute_cmds(&pipes, &saved_std_in, &saved_std_out);
	while (cmd)
	{
		if (cmd->type == 0 || cmd->type == -1)
		{
			look_for_pipe(&has_pipe, cmd);
			if (!cmd->type)
				if (has_pipe)
					pipe(pipes.pipefd);
			if (built_in_cmd(cmd))
				execute_built_in(executor, &pipes, has_pipe, cmd);
			else
				execute_normal_cmd(executor, &pipes, has_pipe, cmd);
		}
		cmd = cmd->next;
	}
	wait_pids_function(executor);
	reset_fds_std(saved_std_in, saved_std_out);
}
