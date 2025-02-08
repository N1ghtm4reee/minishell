/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   syntax_errors.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aakhrif <aakhrif@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/10 15:56:30 by aakhrif           #+#    #+#             */
/*   Updated: 2025/02/08 18:49:15 by aakhrif          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	check_unmatched_quotes(char *s)
{
	int		i;
	char	f_c;
	char	l_c;

	i = -1;
	f_c = 0;
	l_c = 0;
	while (s[++i])
	{
		if (s[i] == '\"' || s[i] == '\'')
		{
			if (!f_c)
				f_c = s[i];
			else
				l_c = s[i];
		}
		if (f_c == l_c)
		{
			f_c = 0;
			l_c = 0;
		}
	}
	if (f_c != l_c)
		return (1);
	return (0);
}

// int	ultimate_check(char *s)
// {
// 	int		i;
// 	int		first;
// 	char	c;

// 	i = -1;
// 	first = 0;
// 	while (s[++i])
// 	{
// 		if (s[i] == '\'' || s[i] == '"')
// 		{
// 			c = s[i++];
// 			while (s[i] && s[i] != c)
// 				i++;
// 			if (!s[i])
// 				return (0);
// 			i++;
// 		}
// 		if (is_special(s[i]) && s[i] != '|')
// 		{
// 	if (first && !ft_isspace(s[i - 1]) && !is_special(s[i - 1]) && s[i] != '|')
// 				return (1);
// 			first = 1;
// 		}
// 	}
// 	return (0);
// }

int	syntax_errors(char *s)
{
	if ((s[0] == '|') || is_special(s[ft_strlen(s) - 1]) == 1)
		return (ft_printf(2, "Syntax error: Invalid Operator place\n"), 1);
	if (check_unmatched_quotes(s))
		return (ft_printf(2, "Syntax error: Unmatched quotes\n"), 1);
	if (check_operator_after_operator(s))
		return (ft_printf(2, "Syntax error: Consecutive operators\n"), 1);
	return (0);
}

	// if (ultimate_check(s))
	// 	return (ft_printf(2, "minishell : syntax error near unexpected token\n")
	// 		, 1);