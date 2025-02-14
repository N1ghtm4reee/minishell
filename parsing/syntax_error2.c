/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   syntax_error2.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aakhrif <aakhrif@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/07 15:11:30 by aakhrif           #+#    #+#             */
/*   Updated: 2025/02/12 22:12:49 by aakhrif          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

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
