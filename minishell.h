/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mlouati <mlouati@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/26 14:41:40 by aakhrif           #+#    #+#             */
/*   Updated: 2025/02/13 18:18:31 by mlouati          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include <fcntl.h>
# include <limits.h>
# include <readline/history.h>
# include <readline/readline.h>
# include <signal.h>
# include <stdbool.h>
# include <stdint.h>
# include <stdio.h>
# include <stdlib.h>
# include <sys/wait.h>
# include <unistd.h>

# define HERE_DOC_MSG1 "minishell: warning: here-document at line "
# define HERE_DOC_MSG2 " delimited by end-of-file (wanted '"
# define HERE_DOC_MSG3 "')\n"

# define HERE_DOC_MAX 16

typedef struct s_expand_params	t_expand_params;
typedef struct s_exp_env		t_exp;

struct							s_gc
{
	void						*p;
	struct s_gc					*next;
};

typedef struct s_env
{
	char						*var_name;
	char						*var_value;
	int							has_eq_ind;
	struct s_env				*next;
}								t_env;

typedef struct sig_hand
{
	int							reading_from_here_doc;
	int							executing;
	int							in_exec;
	pid_t						pid;
}								t_sig;

int								has_quotes(char *s);
int								is_special_str(char *s);
int								syntax_error(char **cmds);
int								check_single_op1(char *str, int *i,
									int *found_op);
int								handle_quotes1(char *str, int *i);
int								check_double_op1(char *str, int *i,
									int *found_op);
typedef struct list
{
	int							here_doc_quote;
	int							type;
	int							fd_in;
	int							fd_out;
	char						**command;
	char						*path;
	struct list					*next;
}								t_list;

typedef struct here_doc
{
	char						*limiter;
	int							fd;
	struct here_doc				*next;
}								t_here_doc;

typedef struct execution
{
	pid_t						pid;
	int							pipefd[2];
	int							next_type;
	int							cur_type;
	int							prev_type;
	int							out_fd;
	int							in_fd;
}								t_execution;

typedef struct pipes
{
	int							pipefd[2];
	int							prev_fd;
	int							prev_fd2;
}								t_pipes;

typedef struct s_pids
{
	pid_t						pid;
	struct s_pids				*next;
}								t_pids;

typedef struct mini_exec
{
	int							core;
	int							has_here_doc;
	int							here_doc_oho;
	// int							exit_status;
	int							here_doc_fd;
	char						**path;
	char						**tokens;
	char						***commands;
	t_list						*commands_list;
	t_env						*env;
	t_pids						*pids;
	char						**envp;
	char						*last_pwd;
	char						*pwd;
	char						*old_pwd;
	// t_execution					*exec;
}								t_exec;

// parsing
void							free_all_in_gc(void);
void							*get_head(void);
void							add_to_gc(void *addr);
void							*gc_malloc(size_t bytes);
int								*here_doc_flag(void);
void							set_here_doc_flag(int status);

// parsing utils
int								is_special(char c);

// exit_status
void							set_exit_status(int status);
int								get_exit_status(void);
int								*exit_status(void);
void							set_exit_status(int status);
void							*sig_handler(void);
void							handle_sigquit_child(int sig_num);

void							handle_ctrlc(int sig_num);
void							handle_ctrlc_child(int sig_num);
void							handle_sigquit(int sig_num);
void							block_sigquit_in_child(void);
void							setup_signals(void);

char							*find_path(char **envp);
int								simple_parsing(char *s, t_exec *executor);
void							execute_command(char *s, t_exec *executor,
									t_env *envp);
char							*get_command(char *cmd, char **paths);

// syntax
int								syntax_errors(char *s);
int								check_operator_after_operator(char *s);

// signals
void							*signal_state(void);

// envir functions

int								equal_flag(char *str);
void							print_env(t_env *my_env);
void							init_env(t_env **my_env, char **env);
void							add_back_env(t_env **my_env, t_env *new);
t_env							*new_env(char *str);
int								env_size(t_env *env);
int								equal_signe(char *str);
char							*get_env_value(t_env *my_env, char *var);

// builtins functions

int								ft_export(char **argument, t_env **my_env);
int								ft_env(char **argument, t_env **env);
int								ft_echo(char **argument);
int								ft_exit(char **argument);
int								ft_pwd(t_exec *executor);
int								ft_cd(char **argument, t_env **env,
									t_exec *executor);
int								ft_unset(char **argument, t_env **my_env);

// export special_case
void							signinthandler(int sig);
void							sig_quit(int sig);
int								is_special_case(char *str);
void							export_special_case(t_env **my_env, char *str);
void							change_var_value_special(t_env **my_env,
									char *str_name, char *str_value);
int								plus_signe_index(char *str);
t_env							*new_env_special(char *str);
int								plus_signe_index(char *str);
void							sort_array(char **env_array);
void							ft_export_alone(t_env **my_env);
bool							is_valid(char *str);
// display
void							ft_printf(int fd, char *s);
// execution
void							execute_normal_child(t_pipes *pipes,
									int has_pipe, t_exec *executor,
									t_list *cmd);
void							execute_built_in_parent(t_exec *executor,
									t_pipes *pipes, int has_pipe, pid_t pid);
void							execute_built_in_child(t_pipes *pipes,
									int has_pipe, t_exec *executor,
									t_list *cmd);

void							exceute_cmds(t_exec *executor);
bool							var_exist(t_env *my_env, char *str);
char							**convert_env(t_env *env);
void							sort_array(char **env_array);
void							change_var_value(t_env **my_env, char *str_name,
									char *str_value);
char							**env_for_execv(t_env *env);
int								handle_redirections(t_list *cmd);
int								cmds_count(t_list *cmds);
void							exec_extern_cmd(t_list *cmd, t_env **my_env);
int								try_execute_command(char **paths, t_list *cmd,
									char *command, t_env **my_env);
void							exec_command(char *cmd_path, t_list *cmd,
									t_env **my_env);
char							*build_command_path(char *path, char *command);
t_list							*get_redir(t_list *cmd, int *f);
char							**get_paths(t_env *my_env);
void							path_ready(t_list *cmd, t_env **my_env);
int								exec_builtin(t_list *cmd, t_env **env,
									t_exec *executor);
bool							built_in_cmd(t_list *cmd);
void							exec_non_bultins(t_pipes *pipe, int *has_pipe,
									t_exec *executor, t_list *cmd);
void							exec_builtin_without_pipe(t_list *cmd,
									t_exec *executor);
void							exec_builtin_with_pipe(t_pipes *pipe,
									int *has_pipe, t_list *cmd,
									t_exec *executor);
void							handle_parent_process(pid_t pid,
									t_exec *executor, t_pipes *pipe,
									int *has_pipe);
void							cmd_not_found(char *cmd);
// utils
int								ft_isspace(char c);
int								ft_isalpha(int c);
int								ft_isdigit(int c);
int								ft_isalnum(int c);
char							**ft_split(char const *s, char c);
char							*ft_strjoin1(char *s1, char *s2);
char							*ft_strdup(const char *s);
int								ft_strlen(char *s);
void							*ft_calloc(size_t count, size_t size);
int								ft_strcmp(char *s1, char *s2);
char							*ft_itoa(int n);
char							*ft_strtrim(char const *s1, char const *set);
int								is_special(char c);
char							*ft_substr(char *s, int start, int len);
char							*ft_substr_malloc(char *s, int start, int len);
char							*ft_strdup_malloc(const char *s);
long long						ft_atoi(char *str, int *error);
// pids

t_pids							*new_pid(pid_t pid_value);
void							add_back_pid(t_pids **pids, t_pids *new);
int								size_pids(t_pids *pids);
void							free_pid(t_pids *pids);
void							reset_fds_std(int saved_std_in,
									int saved_std_out);

void							wait_pids_function(t_exec *executor);
void							check_pipe(t_list *cmd, int *has_pipe);
void							handle_pipes_and_execute(t_list *cmd,
									t_pipes *pipes, t_exec *executor);
int								count_commands(t_list *c);

// parsing
char							**ft_split_pipes(char *s);
char							***ft_split_tokens(char **tokens);
t_list							*parse_list(char ***commands, t_exec *executor);
char							*handle_dollars(char *s);
int								*which_to_expand(char *s, int type);
char							*expand_quotes(char *s, int type);
char							*expand_env_export(t_exec *executor, char *s,
									int *arr, int type);
char							*get_env_variable(t_exec *executor, char *s,
									int start, int *end);
void							handle_here_doc(t_exec *executor);
char							**expand_env(t_exec *executor, char **commands,
									int type);
t_list							*expand(t_exec *executor, t_list *commands);
int								handle_here_doc_error(t_exec *executor,
									char *limiter, int counter,
									t_sig *signal_stat);
t_list							*handle_export_special(t_exec *executor,
									t_list *node);
void							handle_regular_command(t_exec *executor,
									t_list *node, int i, int *arr);
bool							is_dollar_expandable(char quote_char);
t_expand_params					init_params(t_exec *executor, char *s, int *i,
									int *arr);
char							*handle_digit(t_expand_params *params);
char							*handle_exit_status_exp(int *i);
void							here_doc_max_error(void);
char							*handle_dollar_in_here_doc(char *s,
									t_exec *executor, int *i);
int								handle_env_var(t_exp *var, int *i,
									char quote_char);
void							handle_exit_status(t_exp *var, int *i);
void							handle_dollar_digit(char c, char quote_char,
									t_exp *var, int *i);
char							**init_result(void);

// parsing utils
char							*new_str(char *s, int to_skip);
char							*find_in_env(t_env *env, char *str);
int								check_if_limiter(char *s, int i);
int								is_in_arr(int counter, int *arr);
char							*append_str(char c);
char							*append_str(char c);
int								ft_isspace(char c);
char							**ft_split_custom(char *s, char c);
// norm structs
typedef struct norm
{
	int							index;
	int							start;
	int							len;
	char						quote_type;
	int							in_quotes;
	int							j;
}								t_norm;

typedef struct norm_dol
{
	char						c;
	char						*s;
	int							i;
}								t_n_dol;

typedef struct w_t_e
{
	int							i;
	int							j;
	int							counter;
	int							inside;
}								t_wte;

typedef struct s_quotes
{
	char						quote_char;
	int							was_quoted;
}								t_quotes;

typedef struct s_exp_env
{
	int							result_size;
	int							type;
	char						*s;
	char						**result;
	t_exec						*executor;
	char						*new_str;
	t_quotes					quotes;
}								t_exp;

typedef struct s_exn
{
	int							type;
	char						**command;
	int							i;
	int							ii;
	int							*arr;
	char						*expanded;
	char						**new;
}								t_exn;

typedef struct s_exp_env2
{
	int							i;
	int							counter;
	char						quote_char;
	int							was_quoted;
	char						*s;
}								t_exp_env2;

typedef struct s_expand_params
{
	char						*s;
	int							*i;
	int							counter;
	int							*arr;
	int							was_quoted;
	t_exec						*executor;
	char						quote_char;
}								t_expand_params;

typedef struct s_quote_pos
{
	int							start;
	int							end;
}								t_quote_pos;

void							handle_quote_case(char *s, t_quote_pos *arr,
									int *i, int *j);
void							find_quote_positions(char *s, t_quote_pos *arr);

typedef struct s_quote_data
{
	int							inside;
	int							should_expand;
	char						quote_char;
}								t_quote_data;

typedef struct s_split_info
{
	int							start;
	int							end;
	char const					*str;
}								t_split_info;

int								check_operator_syntax(char *str);

#endif