# include "minishell.h"

void ft_cd(char **argument, t_env **my_env)
{
    int ac;
    char old_buff[4096];
    char new_buff[4096];
    char *old_pwd;
    char *new_pwd;

    ac = 0;
    old_pwd = getcwd(old_buff, 4096);
    while(argument[ac])
        ac++;
    if( ac > 2)
    {
        write(1, "bash : cd : too many arguments\n", 31); // need exit status
        exit(1);
    }
    else
    {
        if(ac == 1 || (ac == 2 && !ft_strcmp(argument[1], "~")))
        {
            if (chdir(get_env_value(*my_env,"HOME")) == -1)
                perror(" failed to change to home directory");
        }
        else if (ac == 2 && !ft_strcmp(argument[1], ".."))
        {
             if (chdir("..") == -1)
                perror(" failed to change to parent directory");
        }
        else
        {
            if (chdir(argument[1]) == -1)
                perror(" failed to change to the directory");
        }
    }
    new_pwd = getcwd(new_buff, 4096);
    if(old_pwd)
        change_var_value(my_env, "OLDPWD", old_pwd);
    if(new_pwd)
        change_var_value(my_env, "PWD", new_pwd);

}