/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_list.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aakhrif <aakhrif@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/06 22:10:48 by aakhrif           #+#    #+#             */
/*   Updated: 2025/02/07 15:16:28 by aakhrif          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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
	head->type = check_type(commands[0][0]);
	head->command = commands[0];
	head->next = NULL;
	head->here_doc_quote = 0;
	return (head);
}

t_list	*create_new_node(char **command, int previous_type)
{
	t_list	*new_node;

	new_node = gc_malloc(sizeof(t_list));
	new_node->command = command;
	new_node->next = NULL;
	new_node->here_doc_quote = 0;
	if (previous_type == 2)
		new_node->type = 22;
	else if (previous_type == 3)
		new_node->type = 33;
	else if (previous_type == 4)
		new_node->type = 44;
	else if (previous_type == 5)
		new_node->type = 55;
	else
		new_node->type = check_type(command[0]);
	return (new_node);
}

void	append_node(t_list **list, t_list *new_node)
{
	(*list)->next = new_node;
	*list = new_node;
}

t_list	*parse_list(char ***commands)
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
		new_node = create_new_node(commands[i], previous_type);
		if (!new_node)
			return (NULL);
		append_node(&current, new_node);
		previous_type = new_node->type;
	}
	return (head);
}
