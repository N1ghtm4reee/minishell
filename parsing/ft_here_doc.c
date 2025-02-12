/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_here_doc.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aakhrif <aakhrif@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/07 05:40:41 by aakhrif           #+#    #+#             */
/*   Updated: 2025/02/11 16:44:33 by aakhrif          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

char	*expand_here_doc(char *s, t_exec *executor)
{
	int		i;
	char	*new_str;

	i = 0;
	new_str = ft_strdup("");
	while (s[i])
	{
		if (s[i] == '$')
			new_str = ft_strjoin1(new_str,
					handle_dollar_in_here_doc(s, executor, &i));
		else
			new_str = ft_strjoin1(new_str, append_str(s[i++]));
	}
	return (new_str);
}

int	process_here_doc_line(t_exec *executor, char *line)
{
	if (!executor->here_doc_oho)
	{
		line = handle_dollars(line);
		line = expand_here_doc(line, executor);
	}
	write(executor->here_doc_fd, line, ft_strlen(line));
	write(executor->here_doc_fd, "\n", 1);
	return (1);
}

int	read_here_doc(t_exec *executor, char *limiter, int counter)
{
	t_sig	*signal_stat;
	char	*line;

	signal_stat = sig_handler();
	signal_stat->reading_from_here_doc = 1;
	executor->here_doc_fd = open("here_doc", O_CREAT | O_RDWR | O_TRUNC, 0644);
	while (1)
	{
		line = readline("> ");
		if (!line)
			return (handle_here_doc_error(executor, limiter,
					counter, signal_stat));
		if (line && limiter && ft_strcmp(line, limiter) == 0)
			break ;
		process_here_doc_line(executor, line);
		free(line);
	}
	free(line);
	close(executor->here_doc_fd);
	signal_stat->reading_from_here_doc = 0;
	return (1);
}

int	count_here_docs(t_exec *executor)
{
	t_list	*p;
	int		count;

	p = executor->commands_list;
	count = 0;
	while (p)
	{
		if (p->type == 55)
			count++;
		p = p->next;
	}
	return (count);
}

void	handle_here_doc(t_exec *executor)
{
	t_list		*p;
	static int	counter;
	int			count;

	p = executor->commands_list;
	count = count_here_docs(executor);
	signal(SIGINT, handle_ctrlc_child);
	if (count > HERE_DOC_MAX)
		here_doc_max_error();
	while (p && p->next)
	{
		if (p->type == 5)
		{
			counter++;
			if (read_here_doc(executor, p->next->command[0],
					counter))
				counter++;
		}
		p = p->next;
	}
	exit(0);
}
