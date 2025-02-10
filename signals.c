/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aakhrif <aakhrif@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/10 01:56:24 by aakhrif           #+#    #+#             */
/*   Updated: 2025/02/10 12:33:08 by aakhrif          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	*signal_state(void)
{
	static int	sig_state = 0;

	return (&sig_state);
}

void	*sig_handler(void)
{
	static t_sig	stats;

	return (&stats);
}

void	handle_sigquit_child(int sig_num)
{
	t_sig	*sig_stat;

	(void)sig_num;
	sig_stat = sig_handler();
	(void)sig_stat;
}

void	handle_ctrlc_child(int sig_num)
{
	t_sig	*stats;

	(void)sig_num;
	stats = sig_handler();
	if (stats->reading_from_here_doc && !stats->executing)
	{
		write(1, "\n", 1);
		rl_on_new_line();
		rl_replace_line("", 0);
		stats->reading_from_here_doc = 0;
		exit(130);
	}
}

void	handle_ctrlc(int sig_num)
{
	t_sig	*stats;

	(void)sig_num;
	stats = sig_handler();
	if (stats->executing == 0 && !stats->reading_from_here_doc)
	{
		write(1, "\n", 1);
		rl_on_new_line();
		rl_replace_line("", 0);
		rl_redisplay();
		set_exit_status(130);
	}
	else if (stats->executing == 1 && !stats->reading_from_here_doc)
	{
		write(1, "\n", 1);
		rl_replace_line("", 0);
		rl_redisplay();
		set_exit_status(130);
	}
}
