/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_handle_dollar.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aakhrif <aakhrif@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/07 23:26:42 by aakhrif           #+#    #+#             */
/*   Updated: 2025/02/08 16:46:52 by aakhrif          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char *handle_dollars(char *s, int type)
{
    if (type == 55 | type == 5)
        return s;
    t_n_dol var;

    var.c = -2;
    var.s = NULL;
    var.i = 0;
    char *new;
    while(s[var.i])
    {
        if (s[var.i] == '\'' || s[var.i] == '"')
        {
            var.c = s[var.i++];
            while(s[var.i] && s[var.i] != var.c)
            {
                if (var.c == '"')
                {
                    if (s[var.i] && s[var.i + 1] && s[var.i] == '$' && s[var.i + 1] == '$')
                    {
                        new = new_str(s, var.i);
                        if (!new)
                            return (s);
                        s = new;
                        var.i = 0;
                    }
                }
                var.i++;
            }
            if (!s[var.i])
                break;
        }
        else if (s[var.i] && s[var.i + 1] && s[var.i] == '$' && s[var.i + 1] == '$')
        {
                new = new_str(s, var.i);
                if (!new)
                    return (s);
                s = new;
                var.i = 0;
        }
        else
            var.i++;
    }
    return s;
}


static int is_double_dollar(const char *s, int i)
{
    if (!s || !s[i])  // Check if string exists and current position is valid
        return 0;
    if (s[i] != '$')  // Check if current char is $
        return 0;
    if (!s[i + 1])    // Check if next position exists
        return 0;
    return (s[i + 1] == '$');  // Check if next char is $
}
