/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pids.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mlouati <mlouati@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/09 03:24:36 by mlouati           #+#    #+#             */
/*   Updated: 2025/02/09 16:34:52 by mlouati          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_pids *new_pid(pid_t pid_value)
{
    t_pids *new;
    new = gc_malloc(sizeof(t_pids));
    if (!new)
        return (NULL);
    new->pid = pid_value;
    new ->next = NULL;
    return (new);
}

void add_back_pid(t_pids **pids, t_pids *new)
{
    if (!new || ! pids )
        return ;
    if (!(*pids))
        (*pids) = new;
    else
    {
        t_pids *temp = (*pids);
        while(temp ->next)
        {
            temp = temp -> next;
        }
        temp->next = new;
    }
}
int size_pids(t_pids *pids)
{
    if(!pids)
        return (0);
    int size;
    t_pids * temp = pids;
    while(temp)
    {
        size++;
        temp = temp->next;
    }
    return (size);
}
void    free_pid(t_pids *pids)
{
    t_pids *temp;
    if(!pids)
        return;
    temp = pids;
    t_pids *del = NULL;
    while (temp)
    {
        del = temp;
        temp = temp->next;
        free(del);
    }
}