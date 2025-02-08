/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_expand.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aakhrif <aakhrif@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/07 04:56:49 by aakhrif           #+#    #+#             */
/*   Updated: 2025/02/08 17:07:50 by aakhrif          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	init_struct(t_exn *var, char **cmd, int type)
{
	var->i = -1;
	var->expanded = NULL;
	var->arr = NULL;
	var->ii = -1;
	var->expanded = NULL;
	var->new = NULL;
	var->command = cmd;
	var->type = type;
}

void	process_dollars(char **command, int type)
{
	int	ii;

	ii = -1;
	while (command[++ii])
		command[ii] = handle_dollars(command[ii], type);
}

t_list	*process_command_part(t_exec *executor, t_list *node, t_exn *var)
{
	var->arr = which_to_expand(node->command[var->i], node->type);
	if (ft_strcmp(node->command[0], "export"))
		return (handle_export_special(executor, node, var->arr));
	handle_regular_command(executor, node, var->i, var->arr);
	if (ft_strcmp(node->command[0], "export") == 0)
		node->command[var->i] = node->command[var->i];
	return (node);
}

t_list	*process_command(t_exec *executor, t_list *node)
{
	t_exn	var;
	t_list	*result = NULL;

	init_struct(&var, node->command, node->type);
	while (node->command[++var.i])
	{
		process_dollars(var.command, var.type);
		result = process_command_part(executor, node, &var);
		if (!result)
			return (NULL);
		if (ft_strcmp(node->command[0], "export"))
			break ;
	}
	return (node);
}

t_list	*expand(t_exec *executor, t_list *commands)
{
	t_list	*p;
	char	*s;
	int		should_expand;
	char	*expanded;

	p = commands;
	s = NULL;
	should_expand = 0;
	expanded = NULL;
	while (p)
	{
		p = process_command(executor, p);
		// if (!p)
		// 	return (NULL);
		p = p->next;
	}
	return (commands);
}

// #include "minishell.h"

// char *expand_env_export1(t_exec *executor, char *s, int *arr, int type)
// {
//     if (type == 55)
//         return s;
//     int i = 0;
//     int counter = 0;
//     char *new_str = ft_strdup("");
//     int in_heredoc = 0;
//     char quote_char = 0;
//     int was_quoted = 0;
    
//     while (s[i])
//     {
//         if (s[i] == '\'' || s[i] == '"')
//         {
//             if (!quote_char)
//             {
//                 quote_char = s[i];
//                 was_quoted = 1;
//             }
//             else if (quote_char == s[i])
//                 quote_char = 0;
//             new_str = ft_strjoin1(new_str, (char[]){s[i], '\0'});
//             i++;
//         }
//         else if (s[i] == '$' && (!quote_char || quote_char == '"') && check_if_limiter(s, i))
//         {
//             counter++;
//             if (s[i + 1] == '?')
//             {
//                 int *e = exit_status();
//                 new_str = ft_strjoin1(new_str, ft_itoa(*e));
//                 i += 2;
//             }
//             else if (ft_isdigit(s[i + 1]))
//             {
//                 if (is_in_arr(counter, arr) || was_quoted)
//                     i += 2;
//                 else
//                 {
//                     new_str = ft_strjoin1(new_str, (char[]){s[i], '\0'});
//                     i++;
//                 }
//             }
//             else if (ft_isalpha(s[i + 1]) || s[i + 1] == '_')
//             {
//                 if (is_in_arr(counter, arr) || was_quoted)
//                 {
//                     i++;
//                     char *word = get_env_variable(executor, s, i, &i);
//                     if (word)
//                         new_str = ft_strjoin1(new_str, word);
//                 }
//                 else
//                 {
//                     new_str = ft_strjoin1(new_str, (char[]){s[i], '\0'});
//                     i++;
//                 }
//             }
//             else
//             {
//                 new_str = ft_strjoin1(new_str, (char[]){s[i], '\0'});
//                 i++;
//             }
//         }
//         else
//         {
//             new_str = ft_strjoin1(new_str, (char[]){s[i], '\0'});
//             i++;
//         }
//     }
//     return new_str;
// }

// t_list *expand(t_exec *executor, t_list *commands)
// {
//     t_list *expanded_list = gc_malloc(sizeof(t_list));

//     t_list *p = commands;
//     char *s = NULL;
//     int should_expand = 0;
//     char *expanded = NULL;
//     while(p)
//     {
//         int i = 0;
//         int n_i = 0;
//         char **new = malloc(sizeof(char *) * 1024);
//         while(p->command[i])
//         {
//             int ii = -1;
//             while(p->command[++ii])
//                 p->command[ii] = handle_dollars(p->command[ii], p->type);
//             int *arr = which_to_expand(p->command[i], p->type);
//             if (ft_strcmp(p->command[0], "export"))
//             {
//                 char **new = expand_env(executor, p->command, arr, p->type);
//                 if (!new)
//                     return NULL;
//                 int j = -1;
//                     while(new[++j])
//                     {
//                         if (new[j][0] == '\0')
//                             return NULL;
//                         new[j] = expand_quotes(new[j], p);
//                     }
//                 p->command = new;
//                 break;
//             }
//             else
//             {
//                 p->command[i] = expand_quotes(p->command[i], p);
//                 expanded = expand_env_export1(executor, p->command[i], arr, p->type);
//             }
//             if (ft_strcmp(p->command[0], "export") == 0)
//                 p->command[i] = expanded;
//             i++;
//         }
//         // new[n_i] = NULL;
//         // if (ft_strcmp(p->command[0], "export"))
//         //     p->command = new;
//         p = p->next;
//     }
//     return commands;
// }
