/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_expand2.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aakhrif <aakhrif@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/07 20:48:30 by aakhrif           #+#    #+#             */
/*   Updated: 2025/02/10 12:42:12 by aakhrif          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

bool	is_dollar_expandable(char quote_char)
{
	return ((!quote_char || quote_char == '"'));
}

char	**handle_export_command(t_exec *executor, t_list *node)
{
	char	**n;
	char	**new;
	int		ij[2];

	new = expand_env(executor, node->command, node->type);
	ij[1] = -1;
	while (new && new[++ij[1]])
	{
		if (new[ij[1]] && new[ij[1]][0])
			new[ij[1]] = expand_quotes(new[ij[1]]);
	}
	n = gc_malloc(sizeof(char *) * (ij[1] + 1));
	ij[1] = -1;
	ij[0] = 0;
	while (new && new[++ij[1]])
	{
		while (!node->type && new && new[ij[1]] && !new[ij[1]][0])
			ij[1]++;
		if (!new[ij[1]])
			break ;
		n[ij[0]++] = new[ij[1]];
	}
	n[ij[0]] = NULL;
	return (n);
}

t_list	*handle_export_special(t_exec *executor, t_list *node)
{
	char	**new;

	new = handle_export_command(executor, node);
	node->command = new;
	return (node);
}

char	*expand_quotes_exp(char *expanded)
{
	char	*arg;
	int		i;
	int		first_quote;
	int		j;

	arg = NULL;
	i = 0;
	first_quote = 0;
	j = 0;
	arg = gc_malloc(sizeof(char) * (ft_strlen(expanded) + 1));
	while (expanded[i])
	{
		if (first_quote == 0 && (expanded[i] == '\'' || expanded[i] == '"'))
		{
			first_quote = 1;
			i++;
			continue ;
		}
		if ((expanded[i] == '\'' || expanded[i] == '"') && !expanded[i + 1])
			break ;
		arg[j++] = expanded[i++];
	}
	arg[j] = '\0';
	return (arg);
}

void	handle_regular_command(t_exec *executor, t_list *node, int i, int *arr)
{
	char	*expanded;

	expanded = expand_env_export(executor, node->command[i], arr, node->type);
	if (i == 0)
		node->command[i] = expand_quotes(expanded);
	else
		node->command[i] = expand_quotes_exp(expanded);
}
