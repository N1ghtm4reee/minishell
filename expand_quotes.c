/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_quotes.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aakhrif <aakhrif@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/07 18:14:28 by aakhrif           #+#    #+#             */
/*   Updated: 2025/02/08 16:40:10 by aakhrif          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// char *expand_quotes(char *s, t_list *p)
// {
//     int size = ft_strlen(s);
//     int i = 0;
//     char c = -2;
//     int j = 0;
//     int start = -1;
//     int end = -1;
//     int found = -1;
//     int arr[100][2];
//     int should_expand = 0;
//     while(i < 100)
//     {
//         arr[i][0] = -2;
//         arr[i][1] = -2;
//         i++;
//     }
//     i = 0;
//     int prev_i = -1;
//     int inside = 0;
//     while (s[i])
//     {
//         found = 0;
//         if (s[i] == '\'' || s[i] == '"') 
//         {
//             c = s[i];
//             if (c == '\'')
//                 inside = 0;
//             else if (c == '"')
//                 inside = 1;
//             start = i;
//             i++;
//             while (s[i] && s[i] != c)
//             {
//                 if (s[i] == '$' && inside == 1)
//                     should_expand = 1;
//                 i++;
//             }
//             if (s[i] == c)
//             {
//                 end = i;
//                 arr[j][0] = start;
//                 arr[j][1] = end;
//                 j++;
//             }
//         }
//         else
//         {
//             if (i > 0)
//             {
//                 if (s[i - 1] && s[i - 1] == '\'' || s[i - 1] == '"')
//                     start = i - 1;
//             }
//             while(s[i] && (s[i] != '\'' && s[i] != '"'))
//                 i++;
//             end = i;
//             arr[j][0] = start;
//             arr[j][1] = end;
//             j++;
//             found = 1;
//         }
//         if (!found)
//             i++;
//     }
//     i = 0;
//     j = 0;
//     char *str = ft_strdup("");
//     while(arr[j][0] != -2)
//     {
//         if (p->type == 55)
//             p->here_doc_quote = 1;
//         char *s1 = gc_malloc(sizeof(char) * (arr[j][1] - arr[j][0] + 1));
//         i = arr[j][0] + 1;
//         int k = 0;
//         while(i < arr[j][1])
//         {
//             s1[k++] = s[i++];
//         }
//         s1[k] = '\0';
//         str = ft_strjoin1(str, s1);
//         j++;
//     }
//     if (arr[0][0] == -2)
//         return s;
//     return str;
// }

void init_array(int arr[100][2])
{
    int i = 0;
    while (i < 100)
    {
        arr[i][0] = -2;
        arr[i][1] = -2;
        i++;
    }
}

int process_quote(char *s, int i, char c, int *inside, int *should_expand)
{
    if (c == '\'')
        *inside = 0;
    else if (c == '"')
        *inside = 1;
    i++;
    while (s[i] && s[i] != c)
    {
        if (s[i] == '$' && *inside == 1)
            *should_expand = 1;
        i++;
    }
    return i;
}

int process_non_quote(char *s, int i)
{
    while(s[i] && (s[i] != '\'' && s[i] != '"'))
        i++;
    return i;
}

void find_quote_positions(char *s, int arr[100][2])
{
    int i = 0;
    int j = 0;
    int start = -1;
    int end = -1;
    int inside = 0;
    int should_expand = 0;
    char c;
    
    while (s[i])
    {
        int found = 0;
        if (s[i] == '\'' || s[i] == '"')
        {
            c = s[i];
            start = i;
            i = process_quote(s, i, c, &inside, &should_expand);
            if (s[i] == c)
            {
                end = i;
                arr[j][0] = start;
                arr[j][1] = end;
                j++;
            }
        }
        else
        {
            if ((i > 0 && s[i - 1]) &&  ((s[i - 1] == '\'' || s[i - 1] == '"')))
                start = i - 1;
            i = process_non_quote(s, i);
            end = i;
            arr[j][0] = start;
            arr[j][1] = end;
            j++;
            found = 1;
        }
        if (!found)
            i++;
    }
}

char *create_final_string(char *s, int arr[100][2], t_list *p)
{
    int i = 0;
    int j = 0;
    char *str = ft_strdup("");
    
    while(arr[j][0] != -2)
    {
        if (p->type == 55)
            p->here_doc_quote = 1;
        char *s1 = gc_malloc(sizeof(char) * (arr[j][1] - arr[j][0] + 1));
        i = arr[j][0] + 1;
        int k = 0;
        while(i < arr[j][1])
            s1[k++] = s[i++];
        s1[k] = '\0';
        str = ft_strjoin1(str, s1);
        j++;
    }
    return str;
}

char *expand_quotes(char *s, t_list *p)
{
    int arr[100][2];
    
    init_array(arr);
    find_quote_positions(s, arr);
    
    if (arr[0][0] == -2)
        return s;
        
    return create_final_string(s, arr, p);
}
