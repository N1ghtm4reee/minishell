# include "minishell.h"

int ft_is_number(char *str)
{
    int i;

    i = 0;
    while(str[i])
    {
        if (str[i] < '0' || str[i] > '9')
            return(0);
        i++;
    }
    return(1);
}

void ft_exit(char **argument)
{
    if(!argument[1])
        exit(EXIT_SUCCESS);
    else if (argument[1] && !ft_is_number(argument[1]))
    {
        printf("bash : exit: %s: numeric argument required",argument[1]);
        exit(2);
    }
    else 
    {
        if(!argument[2])
            exit(ft_atoi(argument[1]));
        else
        {
            printf("bash: exit: too many arguments");
            exit(ft_atoi(argument[1]));
        }
    }
}