/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_expand.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aakhrif <aakhrif@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/07 04:56:49 by aakhrif           #+#    #+#             */
/*   Updated: 2025/02/13 15:24:07 by aakhrif          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static void	init_struct(t_exn *var, char **cmd, int type)
{
	var->i = -1;
	var->expanded = NULL;
	var->arr = NULL;
	var->ii = -1;
	var->expanded = NULL;
	var->new = NULL;
	var->command = cmd;
	var->type = type;
}

void	process_dollars(char **command, int type)
{
	int	ii;

	ii = -1;
	while (command[++ii])
	{
		if (type != 55)
			command[ii] = handle_dollars(command[ii]);
	}
}

t_list	*process_command_part(t_exec *executor, t_list *node, t_exn *var)
{
	var->arr = which_to_expand(node->command[var->i], node->type);
	if (ft_strcmp(node->command[0], "export"))
		return (handle_export_special(executor, node));
	handle_regular_command(executor, node, var->i, var->arr);
	if (ft_strcmp(node->command[0], "export") == 0)
		node->command[var->i] = node->command[var->i];
	return (node);
}

t_list	*process_command(t_exec *executor, t_list *node)
{
	t_exn	var;
	t_list	*result;

	result = NULL;
	init_struct(&var, node->command, node->type);
	while (node->command[++var.i])
	{
		process_dollars(var.command, var.type);
		result = process_command_part(executor, node, &var);
		if (!result)
			return (NULL);
		if (ft_strcmp(node->command[0], "export"))
			break ;
	}
	return (node);
}

t_list	*expand(t_exec *executor, t_list *commands)
{
	t_list	*p;

	p = commands;
	while (p)
	{
		p = process_command(executor, p);
		p = p->next;
	}
	return (commands);
}
