/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_env2.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aakhrif <aakhrif@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/07 23:10:43 by aakhrif           #+#    #+#             */
/*   Updated: 2025/02/15 00:26:23 by aakhrif          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

char	*get_env_variable(t_exec *executor, char *s, int start, int *end)
{
	int		i;
	int		j;
	char	*str;

	i = start;
	while (s[i])
	{
		if (ft_isalnum(s[i]) || s[i] == '_')
			i++;
		else
			break ;
	}
	*end = i;
	j = 0;
	str = gc_malloc(sizeof(char) * (i - start + 1));
	while (start < i)
		str[j++] = s[start++];
	str[j] = '\0';
	return (find_in_env(executor->env, str));
}

void	handle_dollar_digit(char c, char quote_char, t_exp *var, int *i)
{
	if (!quote_char || quote_char == '"')
		*i += 2;
	else
	{
		var->new_str = ft_strjoin1(var->new_str, append_str(c));
		(*i)++;
	}
}

static char	*join_word(char *word, t_exp *var)
{
	char	*joined;

	if (!word)
		word = ft_strdup("");
	if (var->new_str && var->new_str[0])
	{
		var->new_str = ft_strjoin1(var->result[var->result_size], var->new_str);
		joined = ft_strjoin1(var->new_str, word);
	}
	else
		joined = ft_strjoin1(var->result[var->result_size], word);
	return (joined);
}

static void	split_and_store(char *joined, t_exp *var)
{
	char	**splited;
	int		j;

	splited = ft_split_custom(joined, ' ');
	var->new_str = ft_strdup("");
	j = 0;
	while (splited[j])
	{
		var->result[var->result_size++] = ft_strdup(splited[j++]);
	}
	var->result_size--;
}

int	handle_env_var(t_exp *var, int *i, char quote_char)
{
	char	*word;

	if (!quote_char || quote_char == '"')
	{
		(*i)++;
		word = get_env_variable(var->executor, var->s, *i, i);
		if (!word && var->type != 0 && var->type != 55)
			return (ft_printf(2, var->s),
				ft_printf(2, ": ambiguous redirect\n"), set_exit_status(1), 0);
		if (word && word[0])
			split_and_store(join_word(word, var), var);
		else if (var->s[*i])
		{
			var->new_str = ft_strjoin1(var->new_str,
					append_str(var->s[*i]));
			(*i)++;
		}
	}
	else
	{
		var->new_str = ft_strjoin1(var->new_str, append_str(var->s[*i]));
		(*i)++;
	}
	return (1);
}
