/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution4.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aakhrif <aakhrif@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/10 01:57:35 by aakhrif           #+#    #+#             */
/*   Updated: 2025/02/10 02:00:35 by aakhrif          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	count_commands(t_list *c)
{
	int	i;

	i = 0;
	while (c)
	{
		if (!c->type)
			i++;
		c = c->next;
	}
	return (i);
}

void	handle_parent_process(pid_t pid, t_exec *executor, t_pipes *pipe,
		int *has_pipe)
{
	t_pids	*new;

	signal(SIGINT, SIG_IGN);
	new = new_pid(pid);
	add_back_pid(&executor->pids, new);
	if (pipe->prev_fd != -1)
		close(pipe->prev_fd);
	if (*has_pipe)
	{
		close(pipe->pipefd[1]);
		pipe->prev_fd = pipe->pipefd[0];
	}
}
