/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   simple_parsing.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aakhrif <aakhrif@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/05 17:36:31 by aakhrif           #+#    #+#             */
/*   Updated: 2025/02/13 15:24:00 by aakhrif          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	here_doc_wait(t_sig *stats, int status)
{
	stats->reading_from_here_doc = 0;
	if (WIFSIGNALED(status))
	{
		if (WTERMSIG(status) == SIGINT)
		{
			set_exit_status(130);
			return (1);
		}
	}
	else if (WIFEXITED(status))
	{
		set_exit_status(WEXITSTATUS(status));
		if (WEXITSTATUS(status) != 0)
			return (1);
	}
	return (0);
}

char	**append_tokens(char **tokens)
{
	char	**new_tokens;
	int		i;
	int		j;

	i = 0;
	while (tokens[i])
		i++;
	new_tokens = gc_malloc(sizeof(char *) * (i + 2));
	if (!new_tokens)
		return (NULL);
	j = 0;
	new_tokens[j++] = ft_strdup("");
	while (tokens[j - 1])
	{
		new_tokens[j] = tokens[j - 1];
		j++;
	}
	new_tokens[j] = NULL;
	return (new_tokens);
}

int	parser(t_exec *executor, char *str)
{
	executor->tokens = ft_split_pipes(str);
	if (syntax_error(executor->tokens))
		return (1);
	executor->tokens = append_tokens(executor->tokens);
	executor->commands = ft_split_tokens(executor->tokens);
	executor->commands_list = parse_list(executor->commands, executor);
	executor->commands_list = expand(executor, executor->commands_list);
	return (0);
}

int	here_doc(t_sig *stats, t_exec *executor)
{
	int	status;

	stats->reading_from_here_doc = 1;
	stats->pid = fork();
	if (!stats->pid)
		handle_here_doc(executor);
	else if (stats->pid > 0)
		return (waitpid(stats->pid, &status, 0),
			here_doc_wait(stats, status));
	return (0);
}

int	simple_parsing(char *s, t_exec *executor)
{
	t_sig	*stats;
	char	*str;

	stats = sig_handler();
	stats->executing = 0;
	str = ft_strtrim(ft_strdup(s), " \t\n\r\n\f");
	if (str && !str[0])
		return (1);
	if (str && str[0])
		add_history(str);
	if (syntax_errors(str))
		return (set_exit_status(2), 1);
	if (parser(executor, str))
		return (set_exit_status(2), 1);
	if (!executor->commands_list)
		return (1);
	executor->here_doc_oho = *(int *)here_doc_flag();
	if (executor->has_here_doc)
		return (here_doc(stats, executor));
	return (0);
}
