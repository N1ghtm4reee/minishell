/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution1.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aakhrif <aakhrif@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/10 17:47:45 by aakhrif           #+#    #+#             */
/*   Updated: 2025/02/12 15:08:19 by aakhrif          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	reset_fds_std(int saved_std_in, int saved_std_out)
{
	dup2(saved_std_in, STDIN_FILENO);
	dup2(saved_std_out, STDOUT_FILENO);
	close(saved_std_in);
	close(saved_std_out);
	unlink("here_doc");
}

void	core_dumped(t_exec *executor)
{
	if (!executor->core)
		write(2, "Quit (core dumped)\n", 19);
	executor->core = 1;
}

void	wait_pids_function(t_exec *executor)
{
	t_pids	*temp;
	t_sig	*stats;
	int		status;

	stats = sig_handler();
	waitpid(executor->pids->pid, &status, 0);
	temp = executor->pids->next;
	while (temp)
	{
		stats->executing = 1;
		waitpid(temp->pid, &status, 0);
		set_exit_status(WEXITSTATUS(status));
		if (WIFSIGNALED(status))
		{
			if ((WTERMSIG(status) == SIGQUIT))
			{
				core_dumped(executor);
				set_exit_status(131);
			}
			else if (WTERMSIG(status) == SIGINT)
				set_exit_status(130);
		}
		temp = temp->next;
	}
	executor->pids = NULL;
}

int	cmds_count(t_list *cmds)
{
	int	i;

	i = 0;
	if (!cmds)
		return (0);
	while (cmds)
	{
		if (cmds->type == 0)
			i++;
		cmds = cmds->next;
	}
	return (i);
}
