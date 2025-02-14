/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_list.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aakhrif <aakhrif@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/06 22:10:48 by aakhrif           #+#    #+#             */
/*   Updated: 2025/02/11 16:44:42 by aakhrif          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	check_type(char *s)
{
	if (ft_strcmp(s, "|") == 0)
		return (1);
	if (ft_strcmp(s, ">>") == 0)
		return (4);
	if (ft_strcmp(s, "<<") == 0)
		return (5);
	if (ft_strcmp(s, ">") == 0)
		return (3);
	if (ft_strcmp(s, "<") == 0)
		return (2);
	return (0);
}

t_list	*initialize_head(char ***commands)
{
	t_list	*head;

	head = gc_malloc(sizeof(t_list));
	head->type = -1;
	head->command = commands[0];
	head->next = NULL;
	return (head);
}

t_list	*create_new_node(char **command, int previous_type, t_exec *executor)
{
	t_list	*new_node;

	new_node = gc_malloc(sizeof(t_list));
	new_node->command = command;
	new_node->next = NULL;
	if (previous_type == 2)
		new_node->type = 22;
	else if (previous_type == 3)
		new_node->type = 33;
	else if (previous_type == 4)
		new_node->type = 44;
	else if (previous_type == 5)
	{
		new_node->type = 55;
		if (has_quotes(command[0]))
			set_here_doc_flag(1);
		executor->has_here_doc = 1;
	}
	else
		new_node->type = check_type(command[0]);
	return (new_node);
}

void	append_node(t_list **list, t_list *new_node)
{
	(*list)->next = new_node;
	*list = new_node;
}

t_list	*parse_list(char ***commands, t_exec *executor)
{
	t_list	*new_node;
	t_list	*head;
	t_list	*current;
	int		i;
	int		previous_type;

	i = 0;
	if (!commands || !commands[0])
		return (NULL);
	head = initialize_head(commands);
	if (!head)
		return (NULL);
	current = head;
	previous_type = head->type;
	while (commands[++i])
	{
		new_node = create_new_node(commands[i], previous_type, executor);
		if (!new_node)
			return (NULL);
		append_node(&current, new_node);
		previous_type = new_node->type;
	}
	return (head);
}
