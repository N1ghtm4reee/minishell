/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_expand2.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aakhrif <aakhrif@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/07 20:48:30 by aakhrif           #+#    #+#             */
/*   Updated: 2025/02/09 13:13:40 by aakhrif          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

bool	is_dollar_expandable(char *s, int i, char quote_char)
{
	return ((!quote_char || quote_char == '"') && check_if_limiter(s, i));
}

char	**handle_export_command(t_exec *executor, t_list *node, int *arr)
{
	char	**n;
	char	**new;
	int		ij[2];

	new = expand_env(executor, node->command, arr, node->type);
	ij[1] = -1;
	while (new && new[++ij[1]])
	{
		if (new[ij[1]] && new[ij[1]][0])
			new[ij[1]] = expand_quotes(new[ij[1]], node);
	}
	n = gc_malloc(sizeof(char *) * (ij[1] + 1));
	ij[1] = -1;
	ij[0] = 0;
	while (new && new[++ij[1]])
	{
		while (new && new[ij[1]] && !new[ij[1]][0])
			ij[1]++;
		if (!new[ij[1]])
			break ;
		n[ij[0]++] = new[ij[1]];
	}
	n[ij[0]] = NULL;
	return (n);
}

t_list	*handle_export_special(t_exec *executor, t_list *node, int *arr)
{
	char	**new;

	new = handle_export_command(executor, node, arr);
	node->command = new;
	return (node);
}

char	*expand_quotes_exp(char *expanded, t_list *node)
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
		node->command[i] = expand_quotes(expanded, node);
	else
		node->command[i] = expand_quotes_exp(expanded, node);
}
