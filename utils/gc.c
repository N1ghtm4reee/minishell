/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   gc.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aakhrif <aakhrif@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/11 17:14:13 by aakhrif           #+#    #+#             */
/*   Updated: 2025/02/12 11:02:12 by aakhrif          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static struct s_gc	*g_head = NULL;

void	add_to_gc(void *addr)
{
	struct s_gc	*mem_node;

	mem_node = malloc(sizeof(struct s_gc));
	if (!mem_node)
		return (free_all_in_gc(), exit(1));
	mem_node->p = addr;
	mem_node->next = g_head;
	g_head = mem_node;
}

void	*gc_malloc(size_t bytes)
{
	void	*p;

	p = malloc(bytes);
	if (!p)
		return (free_all_in_gc(), exit(1), NULL);
	add_to_gc(p);
	return (p);
}

void	free_all_in_gc(void)
{
	struct s_gc	*current;
	struct s_gc	*temp;

	current = g_head;
	while (current)
	{
		free(current->p);
		temp = current;
		current = current->next;
		free(temp);
	}
	g_head = NULL;
}
