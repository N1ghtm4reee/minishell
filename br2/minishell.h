/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aakhrif <aakhrif@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/26 14:41:40 by aakhrif           #+#    #+#             */
/*   Updated: 2025/02/09 00:55:32 by aakhrif          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
#define MINISHELL_H

#include <readline/readline.h>
#include <readline/history.h>
# include <unistd.h>
# include <sys/wait.h>
# include <stdio.h>
# include <stdlib.h>
# include <fcntl.h>
# include <stdint.h>
# include <stdbool.h>
#include <signal.h>
#include <limits.h>

#define HERE_DOC_MSG1 "minishell: warning: here-document at line "
#define HERE_DOC_MSG2 " delimited by end-of-file (wanted '"
#define HERE_DOC_MSG3 "')\n"


#define HERE_DOC_MAX 16
typedef struct s_expand_params t_expand_params;
typedef struct s_exp_env t_exp;

//gc utils0
typedef struct gc
{
    void *p;
    struct gc *next;
}   t_gc;


typedef struct s_env
{
    char    *var_name;
    char    *var_value;
    int     has_eq_ind;
    struct s_env    *next;
}   t_env;

typedef struct sig_hand
{
    int reading_from_here_doc;
    int executing;
    int in_exec;
    pid_t pid;
}   t_sig;

void handle_sigquit_child();

// ls -la | wc -l > output_file

// ls -la -> wc -l -> output_file


// prev = 3 type = 33 next = -1
// prev = 1 type = 0 next = 3
// prev = -1 type = 0 next = 1

typedef struct list
{
    int here_doc_quote;
    int type;//0 if word | 1 if pipe | 2 if red_inp | 3 if red_out_trunc | 4 if red_out_append | 5 if here_doc
    //55 type is the here_doc limiter
    //44 tyoe is the outfile name in append_mode
    //33 tyoe is the outfile name in trunc_mode
    //22 tyoe is the input_file name
    int fd_in;
    int fd_out;
    char **command;
    char *path;
    struct list *next;
}   t_list;


typedef struct here_doc
{
    char *limiter;
    int fd;
    struct here_doc *next;
}   t_here_doc;

typedef struct execution
{
    pid_t pid;
    int pipefd[2];
    int next_type;
    int cur_type;
    int prev_type;
    int out_fd;
    int in_fd;
}   t_execution;

typedef struct pipes
{
    int pipefd[2];
    int prev_fd;
    int prev_fd2;
}   t_pipes;

typedef struct s_pids
{
    pid_t pid;
    struct s_pids *next;
}   t_pids;

typedef struct mini_exec
{
    int exit_status;
    int here_doc_fd;
    char **path;
    char **tokens;
    char ***commands;
    // char **special_chars;
    // int cmd_size;
    t_list *commands_list;
    t_env *env;
    t_pids *pids;
    // char *command_path;
    // char **full_command;
    char **envp;
    char *last_pwd;
    char *pwd;
    char *old_pwd;
    // t_here_doc *here_docs;
    t_execution *exec;
}   t_exec;

//parsing
void free_all_in_gc();
void *get_head();
void add_to_gc(void *addr);
void *gc_malloc(size_t bytes);

//parsing utils
int	is_special(char c);


//exit_status
void *exit_status();
void set_exit_status(int status);
void *sig_handler();
void handle_sigquit_child();

void handle_ctrlc();
void handle_ctrlc_child();
void handle_sigquit();
void block_sigquit_in_child();
void setup_signals();

char	*find_path(char **envp);
int simple_parsing(char *s, t_exec *executor);
void execute_command(char *s, t_exec *executor, t_env *envp);
char	*get_command(char *cmd, char **paths);

//syntax
int syntax_errors(char *s);
int	check_operator_after_operator(char *s);

//signals
void *signal_state();

// envir functions

int equal_flag(char *str);
void print_env(t_env *my_env);
void init_env(t_env **my_env, char **env);
void add_back_env(t_env **my_env, t_env *new);
t_env *new_env(char *str);
int env_size(t_env *env);
int equal_signe(char *str);
char *get_env_value(t_env *my_env, char *var);

// builtins functions

int    ft_export (char **argument, t_env **my_env);
int    ft_env(char  **argument, t_env **env);
int     ft_echo(char **argument);
void    ft_exit(char **argument);
int     ft_pwd(t_exec *executor);
int ft_cd(char **argument, t_env **env, t_exec *executor);
int ft_unset(char **argument, t_env **my_env);

// export special_case

int is_special_case(char *str);
void export_special_case(t_env **my_env, char *str);
void change_var_value_special(t_env **my_env, char *str_name, char *str_value);
int plus_signe_index(char *str);
t_env *new_env_special(char *str);

//display
void	ft_printf(int fd, char *s);

//execution
// void exceute_cmds(t_exec *executor, t_env **env);
void exceute_cmds(t_exec *executor);
bool var_exist(t_env *my_env, char *str);
char **convert_env(t_env *env);
void sort_array(char **env_array);
void change_var_value(t_env **my_env, char *str_name, char *str_value);
char **env_for_execv(t_env *env);
int handle_redirections(t_list *cmd, t_pipes *pipes);

//utils
int ft_isspace(char c);
int     ft_isalpha(int c);
int	    ft_isdigit(int c);
int     ft_isalnum(int c);
char	**ft_split(char const *s, char c);
char	*ft_strjoin1(char *s1, char *s2);
char	*ft_strdup(const char *s);
size_t	ft_strlen(char *s);
void	*ft_calloc(size_t count, size_t size);
int ft_strcmp(char *s1, char *s2);
char	*ft_itoa(int n);
// int is_special_char(char *s);
char	*ft_strtrim(char const *s1, char const *set);
int is_special(char c);
char	*ft_substr(char *s, int start, int len);
// int	ft_atoi(char *str);
char *ft_substr_malloc(char *s, int start, int len);
char	*ft_strdup_malloc(const char *s);
long long ft_atoi(char *str, int *error);
// pids 

t_pids *new_pid(pid_t pid_value);
void add_back_pid(t_pids **pids, t_pids *new);
int size_pids(t_pids *pids);


//parsing
char **ft_split_pipes(char *s);
char ***ft_split_tokens(char **tokens);
t_list *parse_list(char ***commands);
char *handle_dollars(char *s, int type);
int *which_to_expand(char *s, int type);
char *expand_quotes(char *s, t_list *p);
char *expand_env_export(t_exec *executor, char *s, int *arr, int type);
char *get_env_variable(t_exec *executor, char *s, int start, int *end);
void handle_here_doc(t_exec *executor);
char **expand_env(t_exec *executor, char **commands, int *arr, int type);
t_list *expand(t_exec *executor, t_list *commands);
// char *expand_env_export(t_exec *executor, char *s, int *arr, int type);
int	handle_here_doc_error(t_exec *executor, char *limiter,
		int counter, t_sig *signal_stat);
t_list *handle_export_special(t_exec *executor, t_list *node, int *arr);
void handle_regular_command(t_exec *executor, t_list *node, int i, int *arr);
bool is_dollar_expandable(char *s, int i, char quote_char);
t_expand_params	init_params(t_exec *executor, char *s, int *i, int *arr);
char	*handle_digit(t_expand_params *params);
char	*handle_exit_status_exp(int *i);
void	here_doc_max_error(void);
char	*handle_dollar_in_here_doc(char *s, t_exec *executor, int *i);
int	handle_env_var(t_exp *var, int *i, char quote_char);
void	handle_exit_status(t_exp *var, int *i);
void	handle_dollar_digit(char c, char quote_char, t_exp *var, int *i);
char	**init_result(void);


//parsing utils
char *new_str(char *s, int to_skip);
char *find_in_env(t_env *env, char *str);
int check_if_limiter(char *s, int i);
int is_in_arr(int counter, int *arr);
char *append_str(char c);
char *append_str(char c);
int ft_isspace(char c);


// norm structs
typedef struct norm
{
    int index;
    int start;
    int len;
    char quote_type;
    int in_quotes;
    int j;    
}   t_norm;

typedef struct norm_dol
{
    char c;
    int i;
}   t_n_dol;

typedef struct w_t_e
{
    int i;
    int j;
    int counter;
    int inside;
}   t_wte;

typedef struct s_quotes
{
    char    quote_char;
    int     was_quoted;
}   t_quotes;

typedef struct s_exp_env
{
    int         result_size;
    int         type;
    char        *s;
    char        **result;
    t_exec      *executor;
    char        *new_str;
    t_quotes    quotes;
}   t_exp;

typedef struct s_exn
{
    int type;
    char **command;
    int i;
    int ii;
    int *arr;
    char *expanded;
    char **new;
}   t_exn;

typedef struct s_exp_env2
{
    int i;
    int counter;
    char quote_char;
    int was_quoted;
    char *s;
}   t_exp_env2;

typedef struct s_expand_params
{
    char *s;
    int *i;
    int counter;
    int *arr;
    int was_quoted;
    t_exec *executor;
    char quote_char;
}	t_expand_params;

typedef struct s_quote_pos
{
    int	start;
    int	end;
}	t_quote_pos;

typedef struct s_quote_data
{
    int inside;
    int should_expand;
    char quote_char;
}   t_quote_data;

#endif