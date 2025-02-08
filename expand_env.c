/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_env.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aakhrif <aakhrif@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/07 04:21:24 by aakhrif           #+#    #+#             */
/*   Updated: 2025/02/08 17:08:07 by aakhrif          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	handle_quotes(t_exp *var, char c, int *i)
{
	if (!var->quotes.quote_char)
	{
		var->quotes.quote_char = c;
		var->quotes.was_quoted = 1;
	}
	else if (var->quotes.quote_char == c)
		var->quotes.quote_char = 0;
	var->new_str = ft_strjoin1(var->new_str, append_str(c));
	(*i)++;
}

static int	process_dollar(t_exp *var, int *i, char quote_char)
{
	if (var->s[*i + 1] && var->s[*i + 1] == '?')
		handle_exit_status(var, i);
	else if (var->s[*i + 1] && ft_isdigit(var->s[*i + 1]))
		handle_dollar_digit(var->s[*i], quote_char, var, i);
	else if (var->s[*i + 1] && (ft_isalpha(var->s[*i + 1]) || var->s[*i + 1] == '_'))
	{
		if (!handle_env_var(var, i, quote_char))
			return (0);
	}
	else
	{
		var->new_str = ft_strjoin1(var->new_str, append_str(var->s[*i]));
		(*i)++;
	}
	return (1);
}

static int	process_command(t_exp *var)
{
	int	i;

	i = 0;
	while (var->s[i])
	{
		if (var->s[i] == '\'' || var->s[i] == '"')
			handle_quotes(var, var->s[i], &i);
		else if (var->s[i] == '$'
			&& (!var->quotes.quote_char || var->quotes.quote_char == '"'))
		{
            if (!process_dollar(var, &i, var->quotes.quote_char))
                return 0;
		}
		else if (var->s[i] != '"')
		{
			var->new_str = ft_strjoin1(var->new_str,
					append_str(var->s[i]));
			i++;
		}
	}
	return (1);
}

static void	init_struct(t_exp *var, t_exec *executor, int type)
{
	var->new_str = NULL;
	var->result_size = 0;
	var->type = type;
	var->s = NULL;
	var->executor = executor;
	var->new_str = ft_strdup("");
	var->result = init_result();
	var->quotes.quote_char = 0;
	var->quotes.was_quoted = 0;
}

char	**expand_env(t_exec *executor, char **commands, int *arr, int type)
{
	t_exp	var;
	int		k;

	if (type == 55 || type == 5)
		return (commands);
	init_struct(&var, executor, type);
	k = 0;
	while (commands[k])
	{
		var.s = commands[k];
        process_command(&var);
		// if (!process_command(&var))
		// 	return (NULL);
		var.result[var.result_size++] = ft_strjoin1(var.result[var.result_size],
				var.new_str);
		var.new_str = ft_strdup("");
		k++;
	}
	if (var.result)
		var.result[var.result_size] = NULL;
	return (var.result);
}

// char *get_env_variable1(t_exec *executor, char *s, int start, int *end)
// {
//     int i = start;

//     while(s[i])
//     {
//         if (ft_isalnum(s[i]) || s[i] == '_')
//             i++;
//         else
//             break;
//     }
//     *end = i;
//     int j = 0;
//     char *str = malloc(sizeof(char) * (i - start + 1));
//     while(start < i)
//         str[j++] = s[start++];
//     str[j] = '\0';
//     return (find_in_env(executor->env, str));
//     // return (new_env);
// }

// char **expand_env(t_exec *executor, char **commands, int *arr, int type)
// {
//     int count = 0;
//     int dollar_count = 0;
//     int k = 0;
//     int i = 0;
//     int expanded = 0;
//     char *s = NULL;
//         char quote_char = 0;
//     int was_quoted = 0;
//     char *new_str = NULL;
//     new_str = ft_strdup("");
//     char **result = malloc(sizeof(char *) * 1024);
//     int result_size = 0;
//     result[0] = ft_strdup("");
//     while(commands[k])
//     {
//         s = commands[k];
//         i = 0;
//         while(s[i])
//         {
//             expanded = 0;
//             if (s[i] == '\'' || s[i] == '"')
//             {
//                 if (!quote_char)
//                 {
//                     quote_char = s[i];
//                     was_quoted = 1;
//                 }
//                 else if (quote_char == s[i])
//                     quote_char = 0;
//                 new_str = ft_strjoin1(new_str, (char[]){s[i], '\0'});
//                 i++;
//             }
//             else if (s[i] == '$' && (!quote_char || quote_char == '"'))
//             {
//                 dollar_count++;
//                 if (s[i + 1] == '?')
//                 {
//                     int *e = exit_status();
//                     new_str = ft_strjoin1(new_str, ft_itoa(*e));
//                     result[result_size] = ft_strjoin1(result[result_size], ft_strdup(new_str));
//                     free(new_str);
//                     new_str = ft_strdup("");
//                     i += 2;
//                 }
//                 else if (ft_isdigit(s[i + 1]))
//                 {
//                     if ((!quote_char || quote_char == '"'))
//                         i += 2;
//                     else
//                     {
//                         new_str = ft_strjoin1(new_str, (char[]){s[i], '\0'});
//                         i++;
//                     }
//                 }
//                 else if (ft_isalpha(s[i + 1]) || s[i + 1] == '_')
//                 {
//                     if ((!quote_char || quote_char == '"'))
//                     {
//                         i++;
//                         char *word = get_env_variable1(executor, s, i, &i);
//                         if (!word && type != 0)
//                         {
//                             write(2, s, ft_strlen(s));
//                             write(2, ": ambiguous redirect\n", 21);
//                             set_exit_status(1);
//                             return NULL;
//                         }
//                         if (word)
//                         {
//                             char *joined;
//                             if (new_str && new_str[0])
//                             {
//                                 new_str = ft_strjoin1(result[result_size], new_str);
//                                 joined = ft_strjoin1(new_str, word);
//                             }
//                             else
//                                 joined = ft_strjoin1(result[result_size], word);
//                             char **splited = ft_split(joined, ' ');
//                             free(new_str);
//                             new_str = ft_strdup("");
//                             int j = 0;
//                             while(splited[j])
//                                     result[result_size++] = ft_strdup(splited[j++]);
//                             result_size--;
//                         }
//                         else
//                         {
//                             new_str = ft_strjoin1(new_str, (char[]){s[i], '\0'});
//                             i++;
//                         }
//                     }
//                     else
//                     {
//                         new_str = ft_strjoin1(new_str, (char[]){s[i], '\0'});
//                         i++;
//                     }
//                 }
//                 else
//                 {
//                     new_str = ft_strjoin1(new_str, (char[]){s[i], '\0'});
//                     i++;
//                 }
//             }
//             else if(s[i] != '"')
//             {
//                 new_str = ft_strjoin1(new_str, (char[]){s[i], '\0'});
//                 i++;
//             }
//         }
//             result[result_size++] = ft_strjoin1(result[result_size], new_str);
//             free(new_str);
//             new_str = ft_strdup("");
//         k++;
//     }
//     if (result)
//         result[result_size] = NULL;
//     return result;
// }

// char *append_str(char c)
// {
//     char *str = malloc(2);
//     if (!str)
//         return NULL;
//     str[0] = c;
//     str[1] = '\0';
//     return str;
// }

// static void handle_quotes(t_exp *var, char c, int *i)
// {
//     char *tmp;
//     char *append;
    
//     if (!var->quotes.quote_char)
//     {
//         var->quotes.quote_char = c;
//         var->quotes.was_quoted = 1;
//     }
//     else if (var->quotes.quote_char == c)
//         var->quotes.quote_char = 0;
    
//     append = append_str(c);
//     if (!append)
//         return;
    
//     tmp = ft_strjoin1(var->new_str, append);
//     free(append);
//     if (tmp)
//     {
//         free(var->new_str);
//         var->new_str = tmp;
//     }
//     (*i)++;
// }

// static int process_dollar(t_exp *var, int *i, char quote_char)
// {
//     char *tmp;
//     char *append;
    
//     if (var->s[*i + 1] && var->s[*i + 1] == '?')
//         handle_exit_status(var, i);
//     else if (var->s[*i + 1] && ft_isdigit(var->s[*i + 1]))
//         handle_dollar_digit(var->s[*i], quote_char, var, i);
//     else if (var->s[*i + 1] && (ft_isalpha(var->s[*i + 1]) || var->s[*i + 1] == '_'))
//     {
//         if (!handle_env_var(var, i, quote_char))
//             return (0);
//     }
//     else
//     {
//         append = append_str(var->s[*i]);
//         if (!append)
//             return (0);
        
//         tmp = ft_strjoin1(var->new_str, append);
//         free(append);
//         if (tmp)
//         {
//             free(var->new_str);
//             var->new_str = tmp;
//         }
//         (*i)++;
//     }
//     return (1);
// }

// static int process_command(t_exp *var)
// {
//     int i;
//     char *tmp;
//     char *append;
    
//     i = 0;
//     while (var->s[i])
//     {
//         if (var->s[i] == '\'' || var->s[i] == '"')
//             handle_quotes(var, var->s[i], &i);
//         else if (var->s[i] == '$' && (!var->quotes.quote_char || var->quotes.quote_char == '"'))
//         {
//             if (!process_dollar(var, &i, var->quotes.quote_char))
//                 return (0);
//         }
//         else if (var->s[i] != '"')
//         {
//             append = append_str(var->s[i]);
//             if (!append)
//                 return (0);
            
//             tmp = ft_strjoin1(var->new_str, append);
//             free(append);
//             if (tmp)
//             {
//                 free(var->new_str);
//                 var->new_str = tmp;
//             }
//             i++;
//         }
//     }
//     return (1);
// }

// static void *cleanup_result(char **result, int size)
// {
//     int i;
    
//     i = 0;
//     while (i < size)
//     {
//         free(result[i]);
//         i++;
//     }
//     free(result);
//     return NULL;
// }

// static void init_struct(t_exp *var, t_exec *executor, int type)
// {
//     var->new_str = NULL;
//     var->result_size = 0;
//     var->type = type;
//     var->s = NULL;
//     var->executor = executor;
//     var->new_str = ft_strdup("");
//     var->result = init_result();
//     var->quotes.quote_char = 0;
//     var->quotes.was_quoted = 0;
// }

// char **expand_env(t_exec *executor, char **commands, int *arr, int type)
// {
//     t_exp var;
//     int k;
//     char *tmp;
    
//     if (type == 55 || type == 5)
//         return (commands);
        
//     init_struct(&var, executor, type);
//     if (!var.new_str || !var.result)
//         return NULL;
        
//     k = 0;
//     while (commands[k])
//     {
//         var.s = commands[k];
//         if (!process_command(&var))
//         {
//             free(var.new_str);
//             return (cleanup_result(var.result, var.result_size));
//         }
        
//         tmp = ft_strjoin1(var.result[var.result_size], var.new_str);
//         if (!tmp)
//         {
//             free(var.new_str);
//             return (cleanup_result(var.result, var.result_size));
//         }
        
//         free(var.result[var.result_size]);
//         var.result[var.result_size++] = tmp;
        
//         free(var.new_str);
//         var.new_str = ft_strdup("");
//         if (!var.new_str)
//             return (cleanup_result(var.result, var.result_size));
            
//         k++;
//     }
    
//     free(var.new_str);
//     if (var.result)
//         var.result[var.result_size] = NULL;
        
//     return var.result;
// }
