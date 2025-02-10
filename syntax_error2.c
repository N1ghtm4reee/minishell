/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   syntax_error2.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aakhrif <aakhrif@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/07 15:11:30 by aakhrif           #+#    #+#             */
/*   Updated: 2025/02/10 11:37:33 by aakhrif          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// static int	handle_single_operators(char *s, int *i, int *found_op)
// {
// 	char	op_char;

// 	if (is_special(s[*i]))
// 	{
// 		op_char = s[*i];
// 		(*i)++;
// 		while (s[*i] && ft_isspace(s[*i]))
// 			(*i)++;
// 		if (s[*i] && is_special(s[*i]))
// 		{
// 			if (s[*i] && s[*i + 1] && s[*i + 1] != op_char)
// 				return (-1);
// 			if (op_char == '|' && s[*i] == '|')
// 				return (-1);
// 		}
// 		*found_op = 1;
// 		(*i)++;
// 	}
// 	return (0);
// }

// static int	handle_double_operators(char *s, int *i, int *found_op)
// {
// 	if (!s || !i || !found_op || !s[*i])
// 		return (-1);
// 	if (s[*i] == '>' && s[*i + 1] && s[*i + 1] == '>')
// 	{
// 		if (*found_op == 1)
// 			return (-1);
// 		*found_op = 1;
// 		*i += 2;
// 	}
// 	else if (s[*i] == '<' && s[*i + 1] && s[*i + 1] == '<')
// 	{
// 		if (*found_op == 1)
// 			return (-1);
// 		*found_op = 1;
// 		*i += 2;
// 	}
// 	return (0);
// }

// static int	handle_spaces(char *s, int *i, int *found_op)
// {
// 	if (!s || !i || !found_op || !s[*i])
// 		return (-1);
// 	while (s[*i] && ft_isspace(s[*i]))
// 	{
// 		*found_op = 0;
// 		(*i)++;
// 	}
// 	return (0);
// }

static int	skip_spaces(char *str, int *i)
{
	while (str[*i] && (str[*i] == ' ' || str[*i] == '\t'))
		(*i)++;
	return (0);
}

int	check_operator_syntax(char *str)
{
	int	i;
	int	found_op;
	int	result;

	i = 0;
	found_op = 0;
	while (str[i])
	{
		skip_spaces(str, &i);
		if (!str[i])
			break ;
		if (str[i] == '\'' || str[i] == '"')
		{
			if (!handle_quotes1(str, &i))
				return (0);
			continue ;
		}
		result = check_double_op1(str, &i, &found_op);
		if (result)
			return (result);
		result = check_single_op1(str, &i, &found_op);
		if (result)
			return (result);
	}
	return (0);
}
