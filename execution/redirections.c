/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aakhrif <aakhrif@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/09 00:43:17 by mlouati           #+#    #+#             */
/*   Updated: 2025/02/13 14:45:14 by aakhrif          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	handle_input_redirection(t_list *redir, int f)
{
	if (redir && redir->next)
	{
		redir->next->fd_in = open(redir->next->command[0], O_RDONLY, 0644);
		if (redir->next->fd_in < 0)
		{
			perror("Error opening input file");
			set_exit_status(1);
			return (-1);
		}
		if (!f && dup2(redir->next->fd_in, STDIN_FILENO) == -1)
		{
			perror("dup2 failed for input redirection");
			close(redir->next->fd_in);
			return (-1);
		}
		close(redir->next->fd_in);
	}
	return (0);
}

int	handle_output_redirection(t_list *redir, int f)
{
	if (redir && redir->next)
	{
		redir->next->fd_out = open(redir->next->command[0],
				O_WRONLY | O_CREAT | O_TRUNC, 0644);
		if (redir->next->fd_out < 0)
		{
			perror("Error opening output file");
			return (-1);
		}
		if (!f && dup2(redir->next->fd_out, STDOUT_FILENO) == -1)
		{
			perror("dup2 failed for output redirection");
			close(redir->next->fd_out);
			return (-1);
		}
		close(redir->next->fd_out);
	}
	return (0);
}

int	handle_append_redirection(t_list *redir, int f)
{
	if (redir && redir->next)
	{
		redir->next->fd_out = open(redir->next->command[0],
				O_WRONLY | O_CREAT | O_APPEND, 0644);
		if (redir->next->fd_out < 0)
		{
			perror("Error opening append file");
			return (-1);
		}
		if (!f && dup2(redir->next->fd_out, STDOUT_FILENO) == -1)
		{
			perror("dup2 failed for append redirection");
			close(redir->next->fd_out);
			return (-1);
		}
		close(redir->next->fd_out);
	}
	return (0);
}

int	handle_here_doc_redirection(t_list *redir)
{
	if (redir && redir->next)
	{
		redir->next->fd_in = open("here_doc", O_RDONLY);
		if (dup2(redir->next->fd_in, STDIN_FILENO) == -1)
		{
			perror("dup2 failed for here-document redirection");
			return (-1);
		}
		close(redir->next->fd_in);
	}
	return (0);
}

int	handle_redirections(t_list *cmd)
{
	t_list	*redir;
	int		ret;
	int		f;

	f = 0;
	ret = 0;
	redir = get_redir(cmd, &f);
	while (redir && (redir->type >= 2 && redir->type <= 5))
	{
		if (redir->type == 2)
			ret = handle_input_redirection(redir, f);
		else if (redir->type == 3)
			ret = handle_output_redirection(redir, f);
		else if (redir->type == 4)
			ret = handle_append_redirection(redir, f);
		else if (redir->type == 5)
			ret = handle_here_doc_redirection(redir);
		if (ret == -1)
			return (-1);
		redir = redir->next->next;
	}
	return (0);
}
