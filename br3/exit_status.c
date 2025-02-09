/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit_status.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aakhrif <aakhrif@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/15 18:48:12 by aakhrif           #+#    #+#             */
/*   Updated: 2025/02/08 16:34:30 by aakhrif          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	*exit_status(void)
{
	static int	ext_status;

	return (&ext_status);
}

void	set_exit_status(int status)
{
	int	*e;

	e = exit_status();
	*e = status;
}
