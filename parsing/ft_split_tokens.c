/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split_tokens.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aakhrif <aakhrif@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/06 23:09:50 by aakhrif           #+#    #+#             */
/*   Updated: 2025/02/11 16:44:38 by aakhrif          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static int	count_size(char **tokens)
{
	int	i;
	int	count;

	i = 0;
	count = 0;
	while (tokens[i])
	{
		count++;
		i++;
	}
	return (count);
}

static void	handle_quotes(char *token, int *index,
		int *in_quotes, char *quote_type)
{
	if (!(*in_quotes) && (token[*index] == '"' || token[*index] == '\''))
	{
		*quote_type = token[*index];
		*in_quotes = 1;
	}
	else if (*in_quotes && token[*index] == *quote_type)
	{
		*in_quotes = 0;
		*quote_type = 0;
	}
}

static void	copy_subtoken(char *token, int start, int len, char **subtoken)
{
	int	pos;
	int	k;

	*subtoken = gc_malloc(sizeof(char) * (len + 1));
	k = 0;
	pos = start;
	while (pos < start + len)
		(*subtoken)[k++] = token[pos++];
	(*subtoken)[k] = '\0';
}

static void	split_single_token(char *token, char **commands)
{
	t_norm	var;

	var.index = 0;
	var.j = 0;
	while (token[var.index])
	{
		while (token[var.index] && ft_isspace(token[var.index]))
			var.index++;
		if (!token[var.index])
			break ;
		var.start = var.index;
		var.len = 0;
		var.in_quotes = 0;
		var.quote_type = 0;
		while (token[var.index])
		{
			handle_quotes(token, &var.index, &var.in_quotes, &var.quote_type);
			if (!var.in_quotes && ft_isspace(token[var.index]))
				break ;
			var.len++;
			var.index++;
		}
		copy_subtoken(token, var.start, var.len, &commands[var.j++]);
	}
	commands[var.j] = NULL;
}

char	***ft_split_tokens(char **tokens)
{
	char	***commands;
	int		i;

	i = 0;
	commands = gc_malloc(sizeof(char **) * (count_size(tokens) + 1));
	while (tokens[i])
	{
		commands[i] = gc_malloc(sizeof(char *) * (ft_strlen(tokens[i]) + 1));
		split_single_token(tokens[i], commands[i]);
		i++;
	}
	commands[i] = NULL;
	return (commands);
}
