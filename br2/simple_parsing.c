/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   simple_parsing.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aakhrif <aakhrif@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/05 17:36:31 by aakhrif           #+#    #+#             */
/*   Updated: 2025/02/09 00:59:37 by aakhrif          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	list_size(t_list *lst)
{
	int	i;

	i = 0;
	while (lst)
	{
		i++;
		lst = lst->next;
	}
	return (i);
}

int	ft_var_compare(char *str, char *var_name)
{
	int	i;

	i = 0;
	if (ft_strlen(str) != ft_strlen(var_name))
		return (1);
	while ((str[i] && var_name[i]) && str[i] == var_name[i])
		i++;
	return (str[i] - var_name[i]);
}

char	*find_in_env(t_env *env, char *str)
{
	t_env	*p;

	p = env;
	while (p)
	{
		if (ft_var_compare(str, p->var_name) == 0)
			return (p->var_value);
		p = p->next;
	}
	return (NULL);
}

int	is_in_arr(int counter, int *arr)
{
	int	i;

	i = 0;
	while (arr[i] != -2)
	{
		if (arr[i] == counter)
			return (1);
		i++;
	}
	return (0);
}

// char *new_str(char *s, int to_skip)
// {
//     if (s[to_skip] && !s[to_skip + 1])
//         return ft_strdup("");
//     int i = 0;
//     char *str = gc_malloc(sizeof(char) * (ft_strlen(s) + 1));
//     if (!str)
//         return NULL;
//     int j = 0;
//     while(s[i])
//     {
//         if (i == to_skip)
//             i += 2;
//         str[j++] = s[i++];
//     }
//     str[j] = '\0';
//     return str;
// }

int	new_str_check(char *s, int to_skip)
{
	if (s[to_skip] && !s[to_skip + 1])
		return (1);
	if (!s[0])
		return (1);
	return (0);
}

char	*new_str(char *s, int to_skip)
{
	char	*str;
	int		i;
	int		j;

	if (!s)
		return (NULL);
	if (to_skip >= ft_strlen(s))
		return ft_strdup(s);
	if (new_str_check(s, to_skip))
		return (ft_strdup(""));
	str = gc_malloc(sizeof(char) * ft_strlen(s));
	i = 0;
	j = 0;
	while (s[i] && i < ft_strlen(s))
	{
		if (i == to_skip && i + 1 < ft_strlen(s))
		{
			i += 2;
			if (i >= ft_strlen(s))
				break ;
		}
		if (i < ft_strlen(s))
			str[j++] = s[i++];
	}
	str[j] = '\0';
	return (str);
}

int	check_if_limiter(char *s, int i)
{
	i--;
	while (i > 0)
	{
		if (s[i] == '<' && s[i - 1] == '<')
			return (0);
		if (ft_isalnum(s[i]))
			break;
		i--;
	}
	return (1);
}

static char	**split_env_value(char *value)
{
	char	*trimmed;
	char	**split;

	if (!value)
		return (NULL);
	trimmed = ft_strtrim(value, " \t\n");
	split = ft_split(trimmed, ' ');
	return (split);
}

static int	count_expanded_words(char *env_value)
{
	char	**split;
	int		count;

	count = 0;
	split = split_env_value(env_value);
	if (!split)
		return (0);
	while (split[count])
		count++;
	return (count);
}

int	count_words(char *s)
{
	char	c;
	int		i;
	int		count;

	i = 0;
	count = -1;
	while (s[i])
	{
		while (s[i] && s[i] == ' ')//skip spaces
			i++;
		if (s[i])
			count++;
		while (s[i] && s[i] != ' ')
		{
			if (s[i] == '"')
			{
				c = s[i++];
				while (s[i] && s[i] != c)
					i++;
				i++;
			}
			else
				i++;
		}
	}
	return (count);
}

int here_doc_wait(t_sig *stats, int status)
{
	set_exit_status(WEXITSTATUS(status));
	stats->reading_from_here_doc = 0;
	if (WIFSIGNALED(status))
	{
		if (WTERMSIG(status) == SIGINT)
		{
			set_exit_status(WEXITSTATUS(status));
			return (1);
		}
	}
	return (0);
}

int	simple_parsing(char *s, t_exec *executor)
{
	t_sig	*stats;
	char	*str;
	int status;

	stats = sig_handler();
	str = ft_strtrim(ft_strdup(s), " \t\n\r\n\f");
	if (!str[0])
		return (1);
	if (syntax_errors(str))
		return (set_exit_status(2), 1);
	executor->tokens = ft_split_pipes(str);
	executor->commands = ft_split_tokens(executor->tokens);
	executor->commands_list = parse_list(executor->commands);
	executor->commands_list = expand(executor, executor->commands_list);
	// if (!executor->commands_list)
	// 	return (1);
	stats->reading_from_here_doc = 1;
	stats->pid = fork();
	if (!stats->pid)
		handle_here_doc(executor);
	else if (stats->pid > 0)
		waitpid(stats->pid, &status, 0);
	if (here_doc_wait(stats, status))
		return (1);
	return (0);
}

		// if (WEXITSTATUS(status))
        //     return 1;



//  t_list *tmp = executor->commands_list;
//     while(tmp)
//     {
//         int i = 0;
//         int f = 0;
//         while(tmp->command[i])
//         {
//             if (tmp->type == 22 || tmp->type == 33 || tmp->type == 44)
//             {
//                 if (!tmp->command[0] || !tmp->command[0][0])
//                 {
//                     write(2, "minishell : No such file or directory\n", 38);
//                     set_exit_status(1);
//                     return 1;
//                 }
//             }
//             if (ft_strcmp(executor->commands_list->command[0], ".") == 0)
//             {
//                 write(2, "minishell: .: filename argument required\n", 41);
//                 write(2, ".: usage: . filename [arguments]\n", 33);
//                 set_exit_status(2);
//                 return 1;
//             }
//             else if (ft_strcmp(tmp->command[i], "..") == 0)
//             {
//                 if (ft_strcmp(tmp->command[0], "cd"))
//                 {
//                     write(2, "Error : command not found: ..\n", 30);
//                     set_exit_status(127);
//                     return 1;
//                 }
//             }
//             else if (executor->commands_list->command[0][0] == '/')//need to fix
//             {
//                 write(2, "minishell: ", 11);
//                 write(2, tmp->command[i], ft_strlen(tmp->command[i]));
//                 write(2, ": Is a directory\n", 17);
//                 set_exit_status(126);
//                 return 1;
//             }
//             i++;
//         }
//         tmp = tmp->next;
//     }

// 	int i;
    // while(executor->commands_list)
    // {
    //     i = 0;
    //     while(executor->commands_list->command[i])
    //     {
    //         printf("|%s|,", executor->commands_list->command[i++]);
    //     }
    //     printf("\n");
    //     executor->commands_list = executor->commands_list->next;
    // }