/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aakhrif <aakhrif@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/09 22:46:37 by mlouati           #+#    #+#             */
/*   Updated: 2025/02/10 03:05:52 by aakhrif          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	check_pipe(t_list *cmd, int *has_pipe)
{
	t_list	*temp;

	temp = cmd;
	while (temp)
	{
		if (temp->type == 1)
		{
			*has_pipe = 1;
			break ;
		}
		temp = temp->next;
	}
}

void	handle_pipes_and_execute(t_list *cmd, t_pipes *pipes, t_exec *executor)
{
	int	has_pipe;

	has_pipe = 0;
	check_pipe(cmd, &has_pipe);
	if (has_pipe)
		pipe(pipes->pipefd);
	if (built_in_cmd(cmd))
	{
		if (has_pipe)
			exec_builtin_with_pipe(pipes, &has_pipe, cmd, executor);
		else
			exec_builtin_without_pipe(cmd, executor);
	}
	else
		exec_non_bultins(pipes, &has_pipe, executor, cmd);
}

void	exceute_cmds(t_exec *executor)
{
	t_pipes	pipes;
	t_list	*cmd;
	int		has_pipe;
	int		saved_std_in;
	int		saved_std_out;

	cmd = executor->commands_list;
	pipes.prev_fd = -1;
	saved_std_in = dup(STDIN_FILENO);
	saved_std_out = dup(STDOUT_FILENO);
	set_exit_status(0);
	while (cmd)
	{
		if (cmd->type == 0)
			handle_pipes_and_execute(cmd, &pipes, executor);
		cmd = cmd->next;
	}
	wait_pids_function(executor);
	reset_fds_std(saved_std_in, saved_std_out);
}

void	reset_fds_std(int saved_std_in, int saved_std_out)
{
	dup2(saved_std_in, STDIN_FILENO);
	dup2(saved_std_out, STDOUT_FILENO);
	close(saved_std_in);
	close(saved_std_out);
	unlink("here_doc");
}

void	wait_pids_function(t_exec *executor)
{
	t_pids	*temp;
	int		status;

	temp = executor->pids;
	while (temp)
	{
		waitpid(temp->pid, &status, 0);
		set_exit_status(WEXITSTATUS(status));
		if (WIFSIGNALED(status))
		{
			if ((WTERMSIG(status) == SIGQUIT)
				&& count_commands(executor->commands_list) == 1)
			{
				write(2, "Quit (core dumped)\n", 19);
				set_exit_status(131);
			}
			else if (WTERMSIG(status) == SIGINT)
				set_exit_status(130);
		}
		temp = temp->next;
	}
}
