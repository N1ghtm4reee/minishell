/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   syntax_error2.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aakhrif <aakhrif@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/07 15:11:30 by aakhrif           #+#    #+#             */
/*   Updated: 2025/02/08 18:49:47 by aakhrif          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	handle_single_operators(char *s, int *i, int *found_op)
{
	char	op_char;

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

// // Helper function to safely check next character
// static int check_next_char(const char *s, int i, char op_char)
// {
//     if (!s[i])  // If we're at the end of string
//         return 0;
        
//     // Check if next char exists and differs from op_char
//     if (s[i + 1] && s[i + 1] != op_char)
//         return -1;
        
//     // Check for invalid || case
//     if (op_char == '|' && s[i] == '|')
//         return -1;
        
//     return 0;
// }

// static int handle_single_operators(char *s, int *i, int *found_op)
// {
//     char op_char;
    
//     if (!s || !i || !found_op)  // Check for NULL pointers
//         return (-1);
        
//     if (!s[*i])  // Check if we're at end of string
//         return 0;
        
//     if (is_special(s[*i]))
//     {
//         op_char = s[*i];
//         (*i)++;
        
//         // Skip whitespace safely
//         while (s[*i] && ft_isspace(s[*i]))
//             (*i)++;
            
//         if (s[*i] && is_special(s[*i]))
//         {
//             int check_result = check_next_char(s, *i, op_char);
//             if (check_result < 0)
//                 return (-1);
//         }
        
//         *found_op = 1;
//         if (s[*i])  // Only increment if not at end of string
//             (*i)++;
//     }
    
//     return (0);
// }

static int	handle_double_operators(char *s, int *i, int *found_op)
{
	if (s[*i + 1] && s[*i] == '>' && s[*i + 1] == '>')
	{
		if (*found_op == 1)
			return (1);
		*found_op = 1;
		*i += 2;
	}
	else if (s[*i + 1] && s[*i] == '<' && s[*i + 1] == '<')
	{
		if (*found_op == 1)
			return (1);
		*found_op = 1;
		*i += 2;
	}
	return (0);
}

static int	handle_quotes(char *s, int *i)
{
	char	quote_char;

	quote_char = s[*i];
	(*i)++;
	while (s[*i] && s[*i] != quote_char)
		(*i)++;
	if (!s[*i])
		return (0);
	return (1);
}

static int	handle_spaces(char *s, int *i, int *found_op)
{
	while (s[*i] && ft_isspace(s[*i]))
	{
		*found_op = 0;
		(*i)++;
	}
	return (0);
}

int	check_operator_after_operator(char *s)
{
	int	i;
	int	result;
	int	found_op;

	i = -1;
	found_op = 0;
	while (s[++i])
	{
		handle_spaces(s, &i, &found_op);
		if (s[i] == '\'' || s[i] == '"')
		{
			if (!handle_quotes(s, &i))
				return (0);
		}
		if (handle_double_operators(s, &i, &found_op))
			return (1);
		result = handle_single_operators(s, &i, &found_op);
		if (result == -1)
			return (-1);
		else if (result == 1)
			return (1);
		if (!is_special(s[i]))
			found_op = 0;
	}
	return (0);
}
