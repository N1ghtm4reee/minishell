/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pids.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mlouati <mlouati@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/09 03:24:36 by mlouati           #+#    #+#             */
/*   Updated: 2025/02/09 22:26:27 by mlouati          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_pids	*new_pid(pid_t pid_value)
{
	t_pids	*new;

	new = gc_malloc(sizeof(t_pids));
	if (!new)
		return (NULL);
	new->pid = pid_value;
	new->next = NULL;
	return (new);
}

void	add_back_pid(t_pids **pids, t_pids *new)
{
	t_pids	*temp;

	if (!new || !pids)
		return ;
	if (!(*pids))
		(*pids) = new;
	else
	{
		temp = (*pids);
		while (temp->next)
		{
			temp = temp->next;
		}
		temp->next = new;
	}
}

int	size_pids(t_pids *pids)
{
	int		size;
	t_pids	*temp;

	size = 0;
	if (!pids)
		return (0);
	temp = pids;
	while (temp)
	{
		size++;
		temp = temp->next;
	}
	return (size);
}
