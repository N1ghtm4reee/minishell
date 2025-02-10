CC = cc
READLINE =  -lreadline
FLAGS = -Wall -Wextra -Werror
M_SRC = display.c envir.c execution.c execution1.c execution2.c execution3.c execution4.c exit_status.c expand_env.c expand_env2.c expand_env3.c \
		expand_quotes.c expand_quotes1.c ft_cd.c ft_echo.c ft_env.c ft_exit.c ft_expand_env_export.c ft_expand_env_export2.c ft_expand.c ft_expand2.c \
		ft_export_utils.c ft_export_utils2.c ft_export.c ft_handle_dollar.c ft_here_doc.c ft_here_doc2.c ft_itoa.c ft_pwd.c ft_split_tokens.c ft_split.c \
		ft_splite_pipes.c ft_strjoin.c ft_strtrim.c ft_unset.c gc.c main.c parse_list.c parsing_utils.c pids.c redirections.c signals.c simple_parsing.c \
		simple_parsing1.c syntax_error2.c syntax_error3.c syntax_errors.c utils.c utils1.c utils2.c which_to_expand.c
M_OBJ = $(M_SRC:.c=.o)
NAME = minishell

.c.o :
	$(CC) $(FLAGS) -c -o $@ $<

all : $(NAME)

$(NAME) : $(M_OBJ)
	$(CC) $(FLAGS) $(M_OBJ) -o $(NAME) $(READLINE)

clean :
	@rm -f $(M_OBJ) $(B_OBJ)

fclean : clean
	@rm -f $(NAME) 

re : fclean all

.SECONDARY : $(M_OBJ)