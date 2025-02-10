/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit_status.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aakhrif <aakhrif@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/15 18:48:12 by aakhrif           #+#    #+#             */
/*   Updated: 2025/02/10 21:23:51 by aakhrif          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	*exit_status(void)
{
	static int	ext_status;

	return (&ext_status);
}

void	set_exit_status(int status)
{
	*exit_status() = status;
}

int	get_exit_status(void)
{
	return (*exit_status());
}
