/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_handle_dollar.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aakhrif <aakhrif@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/07 23:26:42 by aakhrif           #+#    #+#             */
/*   Updated: 2025/02/11 16:44:31 by aakhrif          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static void	handle_double_quote(char *s, t_n_dol *var, char **str)
{
	char	*new;

	if (s[var->i] && s[var->i + 1] && s[var->i] == '$' && s[var->i + 1] == '$')
	{
		new = new_str(s, var->i);
		if (!new)
			return ;
		*str = new;
		var->i = 0;
	}
}

char	*handle_non_quote(char *s, t_n_dol *var)
{
	s = new_str(s, var->i);
	var->i = 0;
	return (s);
}

static int	is_double_dollar(const char *s, int i)
{
	if (s[i] && s[i + 1] && s[i] == '$' && s[i + 1] == '$')
		return (1);
	return (0);
}

char	*handle_dollars(char *s)
{
	t_n_dol	var;

	var.c = -2;
	var.i = 0;
	while (s[var.i])
	{
		if (s[var.i] == '\'' || s[var.i] == '"')
		{
			var.c = s[var.i++];
			while (s[var.i] && s[var.i] != var.c)
			{
				if (var.c == '"')
					handle_double_quote(s, &var, &s);
				var.i++;
			}
			if (!s[var.i])
				break ;
			var.i++;
		}
		else if (is_double_dollar(s, var.i))
			s = handle_non_quote(s, &var);
		else
			var.i++;
	}
	return (s);
}
