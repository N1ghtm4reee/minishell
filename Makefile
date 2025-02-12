CC = cc
READLINE = -lreadline
FLAGS = -Wall -Wextra -Werror

P_SRC = $(addprefix ./parsing/, ft_split_custom.c expand_env.c expand_env2.c expand_env3.c expand_quotes.c expand_quotes1.c ft_expand_env_export.c ft_expand_env_export2.c  \
		ft_handle_dollar.c ft_here_doc.c ft_here_doc2.c ft_split_tokens.c ft_splite_pipes.c parse_list.c parsing_utils.c ft_expand2.c \
		simple_parsing.c simple_parsing1.c syntax_error2.c syntax_error3.c syntax_errors.c which_to_expand.c ft_expand.c)

E_SRC = main.c $(addprefix ./execution/, envir.c execution.c execution1.c execution2.c execution4.c ft_cd.c ft_echo.c ft_env.c ft_exit.c \
		ft_export_utils.c ft_export_utils2.c ft_export.c   ft_pwd.c ft_unset.c pids.c redirections.c)

UTL_SRC = $(addprefix ./utils/, ft_split.c ft_itoa.c display.c exit_status.c ft_strjoin.c ft_strtrim.c gc.c signals.c utils.c utils1.c utils2.c utils3.c)

E_OBJ = $(E_SRC:.c=.o)

P_OBJ = $(P_SRC:.c=.o)

UTL_OBJ = $(UTL_SRC:.c=.o)

NAME = minishell

.c.o :
	$(CC) $(FLAGS) -c -o $@ $<

all : $(NAME)

$(NAME) : $(P_OBJ) $(E_OBJ) $(UTL_OBJ)
	$(CC) $(FLAGS) $(P_OBJ) $(E_OBJ) $(UTL_OBJ) -o $(NAME) $(READLINE)

clean :
	@rm -f $(P_OBJ) $(E_OBJ) $(UTL_OBJ)

fclean : clean
	@rm -f $(NAME) 

re : fclean all

.SECONDARY : $(P_OBJ) $(E_OBJ) $(UTL_OBJ)
