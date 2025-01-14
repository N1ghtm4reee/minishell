/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   syntax_errors.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aakhrif <aakhrif@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/10 15:56:30 by aakhrif           #+#    #+#             */
/*   Updated: 2025/01/11 21:08:05 by aakhrif          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// ls -la |     | wc -l > out
// ls -la |     | wc -l > > out
int is_special(char c)
{
    if (c == '>' || c == '<' || c == '|')
        return 1;
    return 0;
}

// int quotes_state()
// {
    
// }
// "ls -la" | 'wc -l'
int check_unmatched_quotes(char *s)
{
    int i = 0;
    char f_c = 0;
    char l_c = 0;
    while(s[i])
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
        i++;
    }
    if (f_c != l_c)
        return 1;
    return 0;
}

int check_operator_after_operator(char *s)
{
    int i = 0;
    int found_op = 0;
    while(s[i])
    {
        while(s[i] && s[i] == ' ')
            i++;
        if (s[i + 1] && s[i] == '>' && s[i + 1] == '>')
        {
            if (found_op)
                return 1;
            found_op = 1;
            i += 2;
        }
        else if(s[i + 1] && s[i] == '<' && s[i + 1] == '<')
        {
            if (found_op)
                return 1;
            found_op = 1;
            i += 2;
        }
        else if (is_special(s[i]))
        {
            char c = s[i];
            if (is_special(s[i + 1]))
            {
                if (s[i + 1] != c)
                    return 1;
                else if (c == '|' && s[i + 1] == '|')
                    return 1;
            }
            if (found_op)
                return 1;
            found_op = 1;
            i++;
        }
        else
            found_op = 0;
        i++;
    }
    return 0;
}

int syntax_errors(char *s)
{
    if (is_special(s[0]) == 1 || is_special(s[ft_strlen(s) - 1]) == 1)
    {
        printf("Syntax error: Invalid Operator place\n");
        // free_all_in_gc();
        // exit(2);
        return 1;
    }
    if (check_unmatched_quotes(s)) {
        printf("Syntax error: Unmatched quotes\n");
        // free_all_in_gc();
        // exit(2);
        return 1;
    }
    if (check_operator_after_operator(s)) {
        printf("Syntax error: Consecutive operators\n");
        // free_all_in_gc();
        // exit(2);
        return 1;
    }
    return 0;
}