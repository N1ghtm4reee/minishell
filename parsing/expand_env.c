/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_env.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aakhrif <aakhrif@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/07 04:21:24 by aakhrif           #+#    #+#             */
/*   Updated: 2025/02/12 10:17:48 by aakhrif          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static void	handle_quotes(t_exp *var, char c, int *i)
{
	if (!var->quotes.quote_char)
	{
		var->quotes.quote_char = c;
		var->quotes.was_quoted = 1;
	}
	else if (var->quotes.quote_char == c)
		var->quotes.quote_char = 0;
	var->new_str = ft_strjoin1(var->new_str, append_str(c));
	(*i)++;
}

static int	process_dollar(t_exp *var, int *i, char quote_char)
{
	if (var->s[*i + 1] && var->s[*i + 1] == '?')
		handle_exit_status(var, i);
	else if (var->s[*i + 1] && ft_isdigit(var->s[*i + 1]))
		handle_dollar_digit(var->s[*i], quote_char, var, i);
	else if (var->s[*i + 1] && (ft_isalpha(var->s[*i + 1])
			|| var->s[*i + 1] == '_'))
	{
		if (!handle_env_var(var, i, quote_char))
			return (0);
	}
	else
	{
		var->new_str = ft_strjoin1(var->new_str, append_str(var->s[*i]));
		(*i)++;
	}
	return (1);
}

static int	process_command(t_exp *var)
{
	int	i;

	i = 0;
	while (var->s[i])
	{
		if (var->s[i] == '\'' || var->s[i] == '"')
			handle_quotes(var, var->s[i], &i);
		else if (var->s[i] == '$'
			&& (!var->quotes.quote_char || var->quotes.quote_char == '"'))
		{
			if (!process_dollar(var, &i, var->quotes.quote_char))
				return (0);
		}
		else if (var->s[i] != '"')
		{
			var->new_str = ft_strjoin1(var->new_str,
					append_str(var->s[i]));
			i++;
		}
	}
	return (1);
}

static void	init_struct(t_exp *var, t_exec *executor, int type)
{
	var->new_str = NULL;
	var->result_size = 0;
	var->type = type;
	var->s = NULL;
	var->executor = executor;
	var->new_str = ft_strdup("");
	var->result = init_result();
	var->quotes.quote_char = 0;
	var->quotes.was_quoted = 0;
}

char	**expand_env(t_exec *executor, char **commands, int type)
{
	t_exp	var;
	int		k;

	if (type == 55 || type == 5)
		return (commands);
	init_struct(&var, executor, type);
	k = 0;
	while (commands[k])
	{
		var.s = commands[k];
		process_command(&var);
		var.result[var.result_size] = ft_strjoin1(var.result[var.result_size],
				var.new_str);
		var.result_size++;
		var.new_str = ft_strdup("");
		k++;
	}
	if (var.result)
		var.result[var.result_size] = NULL;
	return (var.result);
}
