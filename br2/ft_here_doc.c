/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_here_doc.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aakhrif <aakhrif@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/07 05:40:41 by aakhrif           #+#    #+#             */
/*   Updated: 2025/02/08 17:29:53 by aakhrif          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// char	*handle_dollar_in_here_doc(char *s, t_exec *executor, int *i)
// {
// 	char	*word;
// 	char	*new_str;
// 	int		*e;

// 	new_str = ft_strdup("");
// 	if (s[*i + 1] == '?')
// 	{
// 		e = exit_status();
// 		new_str = ft_strjoin1(new_str, ft_itoa(*e));
// 		*i += 2;
// 	}
// 	else if (ft_isdigit(s[*i + 1]))
// 	{
// 		(*i)++;
// 		new_str = ft_strjoin1(new_str, (char[]){s[++(*i)], '\0'});
// 		(*i)++;
// 	}
// 	else if (ft_isalpha(s[*i + 1]) || s[*i + 1] == '_')
// 	{
// 		(*i)++;
// 		word = get_env_variable(executor, s, *i, i);
// 		if (word)
// 			new_str = ft_strjoin1(new_str, word);
// 	}
// 	else
// 	{
// 		new_str = ft_strjoin1(new_str, (char[]){s[*i], '\0'});
// 		(*i)++;
// 	}
// 	return (new_str);
// }

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

int	process_here_doc_line(t_exec *executor, char *line,
		char *limiter, int here_doc_quote)
{
	if (!here_doc_quote)
	{
		line = handle_dollars(line, 0);
		line = expand_here_doc(line, executor);
	}
	write(executor->here_doc_fd, line, ft_strlen(line));
	write(executor->here_doc_fd, "\n", 1);
	return (1);
}

int	read_here_doc(t_exec *executor, char *limiter,
		int counter, int here_doc_quote)
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
		if (ft_strcmp(line, limiter) == 0)
			break ;
		process_here_doc_line(executor, line, limiter, here_doc_quote);
	}
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
	t_sig		*stat;
	static int	counter;
	int			count;

	p = executor->commands_list;
	stat = signal_state();
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
					counter, p->next->here_doc_quote))
				counter = counter++;
		}
		p = p->next;
	}
	exit(0);
}
