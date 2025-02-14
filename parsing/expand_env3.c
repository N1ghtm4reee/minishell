/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_env3.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aakhrif <aakhrif@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/07 23:17:07 by aakhrif           #+#    #+#             */
/*   Updated: 2025/02/11 16:44:16 by aakhrif          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

char	**init_result(void)
{
	char	**result;
	int		i;

	result = gc_malloc(sizeof(char *) * 1024);
	i = 0;
	while (i < 1024)
		result[i++] = ft_strdup("");
	return (result);
}

void	handle_exit_status(t_exp *var, int *i)
{
	int	e;

	e = get_exit_status();
	var->new_str = ft_strjoin1(var->new_str, ft_itoa(e));
	var->result[var->result_size] = ft_strjoin1(var->result[var->result_size],
			ft_strdup(var->new_str));
	var->new_str = ft_strdup("");
	*i += 2;
}
