/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mlouati <mlouati@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/09 00:43:17 by mlouati           #+#    #+#             */
/*   Updated: 2025/02/09 16:32:37 by mlouati          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	handle_input_redirection(t_list *redir)
{
	redir->next->fd_in = open(redir->next->command[0], O_RDONLY, 0644);
	if (redir->next->fd_in < 0)
	{
		perror("Error opening input file");
		return (-1);
	}
	if (dup2(redir->next->fd_in, STDIN_FILENO) == -1)
	{
		perror("dup2 failed for input redirection");
		close(redir->next->fd_in);
		return (-1);
	}
	close(redir->next->fd_in);
	return (0);
}
int	handle_output_redirection(t_list *redir)
{
	redir->next->fd_out = open(redir->next->command[0],
			O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (redir->next->fd_out < 0)
	{
		perror("Error opening output file");
		return (-1);
	}
	if (dup2(redir->next->fd_out, STDOUT_FILENO) == -1)
	{
		perror("dup2 failed for output redirection");
		close(redir->next->fd_out);
		return (-1);
	}
	close(redir->next->fd_out);
	return (0);
}
int	handle_append_redirection(t_list *redir)
{
	redir->next->fd_out = open(redir->next->command[0],
			O_WRONLY | O_CREAT | O_APPEND, 0644);
	if (redir->next->fd_out < 0)
	{
		perror("Error opening append file");
		return (-1);
	}
	if (dup2(redir->next->fd_out, STDOUT_FILENO) == -1)
	{
		perror("dup2 failed for append redirection");
		close(redir->next->fd_out);
		return (-1);
	}
	close(redir->next->fd_out);
	return (0);
}
int	handle_here_doc_redirection(t_list *redir)
{
	redir->next->fd_in = open("here_doc", O_RDONLY);
	if (dup2(redir->next->fd_in, STDIN_FILENO) == -1)
	{
		perror("dup2 failed for here-document redirection");
		return (-1);
	}
	close(redir->next->fd_in);
	return (0);
}

int	handle_redirections(t_list *cmd)
{
	t_list	*redir;

	redir = cmd->next;
	while (redir && (redir->type >= 2 && redir->type <= 5))
	{
		if (redir->type == 2) // Input redirection
		{
			if (handle_input_redirection(redir) == -1)
				return (-1);
		}
		else if (redir->type == 3) // Output redirection (>)
		{
			if (handle_output_redirection(redir) == -1)
				return (-1);
		}
		else if (redir->type == 4) // Append (>>)
		{
			if (handle_append_redirection(redir) == -1)
				return (-1);
		}
		else if (redir->type == 5) // Here-document (<<)
		{
			if (handle_here_doc_redirection(redir) == -1)
				return (-1);
		}
		redir = redir->next->next;
	}
	return (0);
}
