/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   syntax_errors.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aakhrif <aakhrif@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/10 15:56:30 by aakhrif           #+#    #+#             */
/*   Updated: 2025/02/12 22:11:49 by aakhrif          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	check_unmatched_quotes(char *s)
{
	int		i;
	char	f_c;
	char	l_c;

	i = -1;
	f_c = 0;
	l_c = 0;
	while (s[++i])
	{
		if (s[i] == '\"' || s[i] == '\'')
		{
			if (!f_c)
				f_c = s[i];
			else
				l_c = s[i];
		}
		if (f_c == l_c)
		{
			f_c = 0;
			l_c = 0;
		}
	}
	if (f_c != l_c)
		return (1);
	return (0);
}

int	syntax_errors(char *s)
{
	(void)s;
	if ((s[0] == '|') || is_special(s[ft_strlen(s) - 1]) == 1)
		return (ft_printf(2, "Syntax error: Invalid Operator place\n"), 1);
	if (check_unmatched_quotes(s))
		return (ft_printf(2, "Syntax error: Unmatched quotes\n"), 1);
	return (0);
}

static int	check_type(char *s)
{
	if (!ft_strcmp(s, "|"))
		return (1);
	if (!ft_strcmp(s, ">>"))
		return (2);
	if (!ft_strcmp(s, "<<"))
		return (3);
	if (!ft_strcmp(s, ">"))
		return (4);
	if (!ft_strcmp(s, "<"))
		return (5);
	return (0);
}

int	syntax_error(char **cmds)
{
	int	i;

	i = 0;
	while (cmds && cmds[i] && cmds[i + 1])
	{
		if (check_type(cmds[i]) == 1 && check_type(cmds[i + 1]) == 1)
			return (ft_printf(2,
					"Syntax error: Consecutive operators\n"), 1);
		if ((check_type(cmds[i]) != 1 && check_type(cmds[i]) != 0)
			&& (check_type(cmds[i + 1]) == 1))
			return (ft_printf(2,
					"Syntax error: Consecutive operators\n"), 1);
		if ((check_type(cmds[i]) && check_type(cmds[i]) != 1)
			&& check_type(cmds[i + 1]))
			return (ft_printf(2,
					"Syntax error: Consecutive operators\n"), 1);
		i++;
	}
	return (0);
}
