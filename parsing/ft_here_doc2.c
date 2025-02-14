/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_here_doc2.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aakhrif <aakhrif@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/07 23:05:26 by aakhrif           #+#    #+#             */
/*   Updated: 2025/02/11 16:44:36 by aakhrif          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

char	*handle_exit_status_case(int *i)
{
	int	e;

	e = get_exit_status();
	*i += 2;
	return (ft_itoa(e));
}

char	*handle_digit_case(char *s, int *i)
{
	char	*result;
	char	digit_str[2];

	(*i)++;
	digit_str[0] = s[++(*i)];
	digit_str[1] = '\0';
	result = ft_strdup(digit_str);
	(*i)++;
	return (result);
}

char	*handle_alpha_case(t_exec *executor, char *s, int *i)
{
	char	*word;

	(*i)++;
	word = get_env_variable(executor, s, *i, i);
	if (word)
		return (ft_strdup(word));
	return (ft_strdup(""));
}

char	*handle_default_case(char *s, int *i)
{
	char	default_str[2];
	char	*result;

	default_str[0] = s[*i];
	default_str[1] = '\0';
	result = ft_strdup(default_str);
	(*i)++;
	return (result);
}

char	*handle_dollar_in_here_doc(char *s, t_exec *executor, int *i)
{
	char	*word;
	char	*new_str;
	char	*temp;

	new_str = ft_strdup("");
	if (s[*i + 1] == '?')
		temp = handle_exit_status_case(i);
	else if (ft_isdigit(s[*i + 1]))
		temp = handle_digit_case(s, i);
	else if (ft_isalpha(s[*i + 1]) || s[*i + 1] == '_')
		temp = handle_alpha_case(executor, s, i);
	else
		temp = handle_default_case(s, i);
	word = ft_strjoin1(new_str, temp);
	return (word);
}
