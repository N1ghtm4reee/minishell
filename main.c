/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aakhrif <aakhrif@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/26 14:42:23 by aakhrif           #+#    #+#             */
/*   Updated: 2025/01/14 11:20:10 by aakhrif          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int main(int ac, char **av, char **envp)
{
    if (ac != 1)
        return 1;
    //get_path
    t_exec executor;
    executor.path = NULL;
    executor.envp = envp;
    t_env *my_env = NULL;
    init_env(&my_env, envp);
    executor.env = my_env;
    // executor.special_chars = get_special_chars();
    while(1)
    {
        //CTRL D | CTRL C
        //
        //read line
        char *s = readline("minishell > ");
        if (s)
        {
            if (simple_parsing(s, &executor) == 0)
                execute_command(s, &executor, envp);
        }
        free_all_in_gc();
        add_history(s);
        free(s);
    }
}
