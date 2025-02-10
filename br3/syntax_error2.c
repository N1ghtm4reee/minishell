/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   syntax_error2.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aakhrif <aakhrif@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/07 15:11:30 by aakhrif           #+#    #+#             */
/*   Updated: 2025/02/10 03:00:21 by aakhrif          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	handle_single_operators(char *s, int *i, int *found_op)
{
	char op_char;

	if (is_special(s[*i]))
	{
		op_char = s[*i];
		(*i)++;
		while (s[*i] && ft_isspace(s[*i]))
			(*i)++;
		if (s[*i] && is_special(s[*i]))
		{
			if (s[*i] && s[*i + 1] && s[*i + 1] != op_char)
				return (-1);
			if (op_char == '|' && s[*i] == '|')
				return (-1);
		}
		*found_op = 1;
		(*i)++;
	}
	return (0);
}

static int	handle_double_operators(char *s, int *i, int *found_op)
{
	if (!s || !i || !found_op || !s[*i])
		return (-1);
	if (s[*i] == '>' && s[*i + 1] && s[*i + 1] == '>')
	{
		if (*found_op == 1)
			return (-1);
		*found_op = 1;
		*i += 2;
	}
	else if (s[*i] == '<' && s[*i + 1] && s[*i + 1] == '<')
	{
		if (*found_op == 1)
			return (-1);
		*found_op = 1;
		*i += 2;
	}
	return (0);
}

// static int	handle_quotes(char *s, int *i)
// {
// 	char quote_char;

// 	quote_char = s[*i];
// 	if (!s || !i || !s[*i])
// 		return (0);
// 	(*i)++;
// 	while (s[*i] && s[*i] != quote_char)
// 		(*i)++;
// 	if (!s[*i])
// 		return (0);
// 	(*i)++;
// 	return (1);
// }

static int	handle_spaces(char *s, int *i, int *found_op)
{
	if (!s || !i || !found_op || !s[*i])
		return (-1);
	while (s[*i] && ft_isspace(s[*i]))
	{
		*found_op = 0;
		(*i)++;
	}
	return (0);
}

// int	check_operator_after_operator(char *s)
// {
// 	int i = 0;
// 	int result;
// 	int found_op = 0;

// 	if (!s)
// 		return (-1);
// 	while (s[i])
// 	{
// 		if (handle_spaces(s, &i, &found_op) == -1)
// 			return (-1);
// 		if (!s[i])
// 			break;
// 		if ((s[i] == '\'' || s[i] == '"') && !handle_quotes(s, &i))
// 			return (0);
// 		if ((result = handle_double_operators(s, &i, &found_op)) != 0)
// 			return (result);
// 		if (s[i] && (result = handle_single_operators(s, &i, &found_op)) != 0)
// 			return (result);
// 		if (s[i] && !is_special(s[i]))
// 			found_op = 0;
// 		if (s[i])
// 			i++;
// 	}
// 	return (0);
// }

static int is_operator(char c)
{
    return (c == '>' || c == '<' || c == '|');
}

static int skip_spaces(char *str, int *i)
{
    while (str[*i] && (str[*i] == ' ' || str[*i] == '\t'))
        (*i)++;
    return (0);
}

static int handle_quotes(char *str, int *i)
{
    char quote;

    quote = str[*i];
    (*i)++;
    while (str[*i] && str[*i] != quote)
        (*i)++;
    if (!str[*i])
        return (0);
    (*i)++;
    return (1);
}

static int check_double_op(char *str, int *i, int *found_op)
{
    if ((str[*i] == '>' && str[*i + 1] == '>') ||
        (str[*i] == '<' && str[*i + 1] == '<'))
    {
        if (*found_op)
            return (1);
        *found_op = 1;
        *i += 2;
    }
    return (0);
}

static int check_single_op(char *str, int *i, int *found_op)
{
    if (is_operator(str[*i]))
    {
        if (*found_op)
            return (1);
        *found_op = 1;
        (*i)++;
    }
    else if (str[*i])
    {
        *found_op = 0;
        (*i)++;
    }
    return (0);
}

int check_operator_syntax(char *str)
{
    int i;
    int found_op;
    int result;

    i = 0;
    found_op = 0;
    while (str[i])
    {
        skip_spaces(str, &i);
        if (!str[i])
            break;
        if (str[i] == '\'' || str[i] == '"')
        {
            if (!handle_quotes(str, &i))
                return (0);
            continue;
        }
        if ((result = check_double_op(str, &i, &found_op)))
            return (result);
        if ((result = check_single_op(str, &i, &found_op)))
            return (result);
    }
    return (0);
}
