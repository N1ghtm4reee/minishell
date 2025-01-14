#include "minishell.h"

char *get_env_value(t_env *my_env, char *var)
{
    t_env *temp;

    temp = my_env;
    while(temp)
    {
        if(!ft_strcmp(temp->var_name, var))
            return(temp->var_value);
        temp = temp->next;
    }
    return(NULL);
}
void ft_pwd(t_env **my_env)
{
    char *pwd;

    pwd = get_env_value(*my_env, "PWD");
    if(pwd)
    {
        write(1, pwd, ft_strlen(pwd));
        write(1, "\n", 1);
    }
    else
        perror("no value for pwd");
}