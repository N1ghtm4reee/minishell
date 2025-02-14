/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_expand2.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aakhrif <aakhrif@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/07 20:48:30 by aakhrif           #+#    #+#             */
/*   Updated: 2025/02/13 15:24:25 by aakhrif          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

bool	is_dollar_expandable(char quote_char)
{
	return ((!quote_char || quote_char == '"'));
}

char	*expand_quotes_custom(char *s, int type)
{
	char		*new_str;
	static int	found_quote;
	static char	quote_type;
	int			i;

	if ((!ft_strcmp(s, "\'\'") || !ft_strcmp(s, "\"\"")) && !type)
		return (ft_strdup(" "));
	new_str = ft_strdup("");
	i = -1;
	while (s[++i])
	{
		if (!found_quote && (s[i] == '\'' || s[i] == '"'))
		{
			found_quote = 1;
			quote_type = s[i];
		}
		else if (found_quote && s[i] == quote_type)
		{
			found_quote = 0;
			quote_type = 0;
		}
		else
			new_str = ft_strjoin1(new_str, append_str(s[i]));
	}
	return (new_str);
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
			new[ij[1]] = expand_quotes_custom(new[ij[1]], node->type);
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

void	handle_regular_command(t_exec *executor, t_list *node, int i, int *arr)
{
	char	*expanded;

	expanded = expand_env_export(executor, node->command[i], arr, node->type);
	node->command[i] = expand_quotes(expanded, node->type);
}
