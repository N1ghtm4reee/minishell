/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_expand_env_export.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aakhrif <aakhrif@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/07 19:03:20 by aakhrif           #+#    #+#             */
/*   Updated: 2025/02/11 16:44:22 by aakhrif          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	handle_quote_state(char c, char *quote_char, int *was_quoted)
{
	if (!*quote_char)
	{
		*quote_char = c;
		*was_quoted = 1;
	}
	else if (*quote_char == c)
		*quote_char = 0;
}

char	*handle_alpha_underscore(t_expand_params *params)
{
	char	*word;

	if (is_in_arr(params->counter, params->arr) || params->was_quoted)
	{
		(*(params->i))++;
		word = get_env_variable(params->executor,
				params->s, *(params->i), params->i);
		if (word)
			return (ft_strdup(word));
		return (ft_strdup(""));
	}
	(*(params->i))++;
	return (append_str(params->s[*(params->i) - 1]));
}

char	*process_dollar(t_expand_params *params)
{
	if (params->s[*(params->i) + 1] && params->s[*(params->i) + 1] == '?')
		return (handle_exit_status_exp(params->i));
	else if (params->s[*(params->i) + 1]
		&& ft_isdigit(params->s[*(params->i) + 1]))
		return (handle_digit(params));
	else if ((params->s[*(params->i) + 1]
			&& ft_isalpha(params->s[*(params->i) + 1]))
		|| params->s[*(params->i) + 1] == '_')
		return (handle_alpha_underscore(params));
	(*(params->i))++;
	return (append_str(params->s[*(params->i) - 1]));
}

char	*process_char(t_expand_params *params)
{
	char	*result;

	if (params->s[*(params->i)] == '\'' || params->s[*(params->i)] == '"')
		handle_quote_state(params->s[*(params->i)],
			&params->quote_char, &params->was_quoted);
	result = append_str(params->s[*(params->i)]);
	(*(params->i))++;
	return (result);
}

char	*expand_env_export(t_exec *executor, char *s, int *arr, int type)
{
	int				i;
	char			*new_str;
	char			*temp;
	t_expand_params	params;

	if (type == 55)
		return (s);
	new_str = ft_strdup("");
	params = init_params(executor, s, &i, arr);
	i = 0;
	while (s[i])
	{
		if (s[i] == '$' && (!params.quote_char || params.quote_char == '"'))
		{
			params.counter++;
			temp = process_dollar(&params);
		}
		else
			temp = process_char(&params);
		new_str = ft_strjoin1(new_str, temp);
	}
	return (new_str);
}
