/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution1.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aakhrif <aakhrif@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/10 17:47:45 by aakhrif           #+#    #+#             */
/*   Updated: 2025/02/10 20:26:51 by aakhrif          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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
		if (WIFEXITED(status))
			set_exit_status(WEXITSTATUS(status));
		if (WIFSIGNALED(status))
		{
			if ((WTERMSIG(status) == SIGQUIT))
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
