/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aakhrif <aakhrif@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/07 18:08:01 by aakhrif           #+#    #+#             */
/*   Updated: 2025/02/09 13:05:49 by aakhrif          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	is_special(char c)
{
	if (c == '>' || c == '<' || c == '|')
		return (1);
	return (0);
}

void *here_doc_flag()
{
	static int here_doc_oho;

	return (&here_doc_oho);
}

void set_here_doc_flag(int status)
{
	int *f = here_doc_flag();

	*f = status;
}

int has_quotes(char *s)
{
	int i = 0;
	while(s[i])
	{
		if (s[i] == '\'' || s[i] == '"')
			return (1);
		i++;
	}
	return (0);
}
