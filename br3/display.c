/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   display.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aakhrif <aakhrif@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/07 17:49:19 by aakhrif           #+#    #+#             */
/*   Updated: 2025/02/09 02:42:24 by aakhrif          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ft_printf(int fd, char *s)
{
	if (!s)
		return ;
	write(fd, s, ft_strlen(s));
}

int	handle_here_doc_error(t_exec *executor, char *limiter,
		int counter, t_sig *signal_stat)
{
	printf(HERE_DOC_MSG1);
	printf("%d", counter);
	printf(HERE_DOC_MSG2);
	printf("%s", limiter);
	printf(HERE_DOC_MSG3);
	close(executor->here_doc_fd);
	signal_stat->reading_from_here_doc = 0;
	return (0);
}

void	here_doc_max_error(void)
{
	ft_printf(2, "to many here_docs\n");
	set_exit_status(2);
	exit(2);
}
