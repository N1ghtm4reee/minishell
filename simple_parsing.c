/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   simple_parsing.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aakhrif <aakhrif@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/05 17:36:31 by aakhrif           #+#    #+#             */
/*   Updated: 2025/01/14 11:14:55 by aakhrif          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*get_command(char *cmd, char **paths)
{
	char	*tmp;
	char	*command;
	int		i;

	i = 0;
	while (paths[i])
	{
		tmp = ft_strjoin1(paths[i], "/");
		command = ft_strjoin1(tmp, cmd);
		free(tmp);
		if (access(command, F_OK) == 0)
			return (command);
		free(command);
		i++;
	}
	return (NULL);
}

int count_size(char **tokens)
{
	int i = 0;
	int count = 0;
	while(tokens[i])
	{
		count++;
        i++;
    }
	return (count);
}

int count_tokens(char *s)
{
    int i = 0;
    int tokens = 0;
    
    while (s[i])
    {
        while (s[i] && s[i] == ' ')
            i++;
        if (!s[i])
            break;
            
        tokens++;
        
        while (s[i])
        {
            if (s[i] == '"' || s[i] == '\'')
            {
                char quote = s[i];
                i++;
                while (s[i] && s[i] != quote)
                    i++;
                if (s[i])
                    i++;
            }
            else if (s[i] == ' ')
                break;
            else
                i++;
        }
    }
    return tokens;
}

char ***ft_split_tokens(char **tokens)
{
    char ***commands = gc_malloc(sizeof(char **) * (count_size(tokens) + 1));
    int i = 0;

    while (tokens[i])
    {
        int tokens_inside = count_tokens(tokens[i]);
        commands[i] = gc_malloc(sizeof(char *) * (tokens_inside + 1));
        int j = 0;
        int index = 0;
        
        while (tokens[i][index])
        {
            while (tokens[i][index] && tokens[i][index] == ' ')
                index++;
            if (!tokens[i][index])
                break;
            int start = index;
            int len = 0;
            int in_quotes = 0;
            char quote_type = 0;
            while (tokens[i][index])
            {
                if (!in_quotes && (tokens[i][index] == '"' || tokens[i][index] == '\''))
                {
                    quote_type = tokens[i][index];
                    in_quotes = 1;
                }
                else if (in_quotes && tokens[i][index] == quote_type)
                {
                    in_quotes = 0;
                    quote_type = 0;
                }
                else if (!in_quotes && tokens[i][index] == ' ')
                    break;
                
                len++;
                index++;
            }
            commands[i][j] = gc_malloc(sizeof(char) * (len + 1));
            int k = 0;
            in_quotes = 0;
            for (int pos = start; pos < start + len; pos++)
            {
                if (tokens[i][pos] == '"' || tokens[i][pos] == '\'')
                {
                    if (!in_quotes)
                        in_quotes = 1;
                    else
                        in_quotes = 0;
                    continue;
                }
                commands[i][j][k++] = tokens[i][pos];
            }
            commands[i][j][k] = '\0';
            j++;
        }
        commands[i][j] = NULL;
        i++;
    }
    commands[i] = NULL;
    return commands;
}

int check_type(char *s)
{
    if (ft_strcmp(s, "|") == 0)
        return 1;
    if (ft_strcmp(s, ">>") == 0)
        return 4;
    if (ft_strcmp(s, "<<") == 0)
        return 5;
    if (ft_strcmp(s, ">") == 0)
        return 3;
    if (ft_strcmp(s, "<") == 0)
        return 2;
    return (0);
}

t_list *parse_list(char ***commands)
{
    t_list *head = gc_malloc(sizeof(t_list));
    if (!head)
        return NULL;
    t_list *p = head;
    int i = 0;
    p->type = check_type(commands[i][0]);
    int c = p->type;
    p->command = commands[i++];
    p->next = NULL;
    while(commands[i])
    {
        t_list *new = gc_malloc(sizeof(t_list));
        if (!new)
            return NULL;
        new->command = commands[i];
        if (c == 2)
            new->type = 22;
        else if (c == 3)
            new->type = 33;
        else if (c == 4)
            new->type = 44;
        else if (c == 5)
            new->type = 55;
        else
            new->type = check_type(commands[i][0]);
        c = new->type;
        new->next = NULL;
        p->next = new;
        p = p->next;
        i++;
    }
    return head;
}

// "hello""world"
char **ft_split_pipes(char *s)
{
    int count = 0;
    int i = 0;
    while (s[i])
    {
        while (s[i] == ' ')
            i++;
        if (s[i + 1] && s[i] == '>' && s[i + 1] == '>')
        {
            if (is_special(s[i + 2]))
            {
                printf("Syntax error: Consecutive operators\n");
                exit(2);
            }
            count++;
            i += 2;
        }
        else if (s[i + 1] && s[i] == '<' && s[i + 1] == '<')
        {
            if (is_special(s[i + 2]))
            {
                printf("Syntax error: Consecutive operators\n");
                exit(2);
            }
            count++;
            i += 2;
        }
        else if (s[i] == '|' || s[i] == '>' || s[i] == '<')
        {
            count++;
            i++;
        }
        else if (s[i])
        {
            count++;
            while (s[i] && s[i] != '|' && s[i] != '<' && s[i] != '>')
                i++;
        }
    }
    char **arr = gc_malloc(sizeof(char *) * (count + 1));
    i = 0;
    int k = 0;
    while(k < count)
    {
        while(s[i] && s[i] == ' ')
            i++;
        if (!s[i])
            break;
        int st = i;
        while(s[i] && s[i] != '|' && s[i] != '<' && s[i] != '>')
        {
            char c = s[i];
            if (c == '\"' || c == '\'')
            {
                while(s[i] && s[i] == c)
                    i++;
                while(s[i] && s[i] != c)
                    i++;
            }
            i++;
        }
        int e = i;
        arr[k] = gc_malloc(sizeof(char) * (e - st + 1));
        int j = 0;
        while(st < e)
            arr[k][j++] = s[st++];
        arr[k++][j] = '\0';
        if (s[i + 1] && s[i] == '>' && s[i + 1] == '>')
        {
            arr[k] = gc_malloc(sizeof(char) * 3);
            arr[k][0] = '>';
            arr[k][1] = '>';
            arr[k++][2] = '\0';
            i += 2;
        }
        else if (s[i + 1] && s[i] == '<' && s[i + 1] == '<')
        {
            arr[k] = gc_malloc(sizeof(char) * 3);
            arr[k][0] = '<';
            arr[k][1] = '<';
            arr[k++][2] = '\0';
            i += 2;
        }
        else if (s[i] == '|' || s[i] == '>' || s[i] == '<')
        {
            arr[k] = gc_malloc(sizeof(char) * 2);
            if (s[i] == '|')
                arr[k][0] = '|';
            else if (s[i] == '<')
                arr[k][0] = '<';
            else if (s[i] == '>')
                arr[k][0] = '>';
            arr[k++][1] = '\0';
            i++;
        }
    }
    arr[k] = NULL;
    return arr;
}

int list_size(t_list *lst)
{
    int i = 0;
    while(lst)
    {
        i++;
        lst = lst->next;
    }
    return i;
}

char *expand_in_quotes(char *s, char c)
{
    int i = 0;
    int dollar = 0;
    char *str;
    int counter = 0;
    while(s[i])
    {
        if (s[i] == '$')
            dollar = 1;
        i++;
    }
    i = 0;
    while(s[i])
    {
        if (s[i] != '\'' && s[i] != '\"')
            break;
        i++;
        if (!s[i])
            return NULL;
    }
    i = 0;
    int j = 0;
    if (dollar == 1)
    {
        //state machine to know if should expand or not
        return NULL;
    }
    else
    {
        while(s[i])
        {
            if (s[i] != c)
                counter++;
            i++;
        }
        i = 0;
        str = gc_malloc(sizeof(char) * (counter + 1));
        while(s[i])
        {
            if (s[i] != c)
                str[j++] = s[i];
            i++;
        }
        str[j] = '\0';
    }
    // printf("%s\n", str);
    // printf("%c\n", c);
    // printf("%d\n", counter);
    return str;
}


t_list *expand(t_list *commands)
{
    t_list *expanded_list = gc_malloc(sizeof(t_list));;

    t_list *p = commands;
    char *s = NULL;
    while(p)
    {
        int i = 0;
        // e_p = gc_malloc(sizeof(t_list));
        // if (p->type == 0)
        // {
            while(p->command[i])
            {
                //check if it has a dollar sign or not
                if ((p->command[i][0] == '"' || p->command[i][0] == '\''))
                {
                    // printf("%s|\n", p->command[i]);
                    p->command[i] = expand_in_quotes(p->command[i], p->command[i][0]);
                    // printf("%s\n", p->command[i]);
                }
                // else
                //     s = normal_expand();
                // p->command[i] = s;
                i++;
            }
        // }
        p = p->next;
    }
    return commands;
}
// echo ""''""
int simple_parsing(char *s, t_exec *executor)
{
    char *str = ft_strtrim(s, " \n");
    if (!str[0])
        return 1;
    if (syntax_errors(str))
        return 1;
    // printf("%s\n", str);
    // char *str_expanded = expand(s);
    executor->tokens = ft_split_pipes(str);
    // int i = 0;
    // while(executor->tokens[i])
    // {
    //     printf("%s\n", executor->tokens[i++]);
    // }
	executor->commands = ft_split_tokens(executor->tokens);
    executor->commands_list = parse_list(executor->commands);
    //expand
    executor->commands_list = expand(executor->commands_list);
    // while(executor->commands_list)
    // {
    //     int i = 0;
    //     while(executor->commands_list->command[i])
    //     {
    //         printf("%s,", executor->commands_list->command[i++]);
    //     }
    //     printf("\n");
    //     executor->commands_list = executor->commands_list->next;
    // }
    return (0);
}
