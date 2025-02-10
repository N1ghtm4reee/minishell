/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   simple_parsing.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aakhrif <aakhrif@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/05 17:36:31 by aakhrif           #+#    #+#             */
/*   Updated: 2025/02/10 02:19:02 by aakhrif          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	check_if_limiter(char *s, int i)
{
	i--;
	while (i > 0)
	{
		if (s[i] == '<' && s[i - 1] == '<')
			return (0);
		if (ft_isalnum(s[i]))
			break ;
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

void	parser(t_exec *executor, char *str)
{
	executor->tokens = ft_split_pipes(str);
	executor->commands = ft_split_tokens(executor->tokens);
	executor->commands_list = parse_list(executor->commands);
	executor->commands_list = expand(executor, executor->commands_list);
}

int	simple_parsing(char *s, t_exec *executor)
{
	t_sig	*stats;
	char	*str;
	int		status;

	status = 0;
	stats = sig_handler();
	str = ft_strtrim(ft_strdup(s), " \t\n\r\n\f");
	if (str && !str[0])
		return (1);
	if (str && str[0])
		add_history(str);
	if (syntax_errors(str))
		return (set_exit_status(2), 1);
	parser(executor, str);
	executor->here_doc_oho = *(int *)here_doc_flag();
	if (!executor->commands_list)
		return (1);
	stats->reading_from_here_doc = 1;
	stats->pid = fork();
	if (!stats->pid)
		handle_here_doc(executor);
	else if (stats->pid > 0)
		return (waitpid(stats->pid, &status, 0)
			, here_doc_wait(stats, status));
	return (0);
}
