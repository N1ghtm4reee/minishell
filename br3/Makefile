CC = gcc 
CFLAGS = -Wall -Wextra -Werror 
RM = rm -rf
NAME = minishell

SRCS =	display.c envir.c execution.c exit_status.c expand_env.c\
		expand_env2.c expand_env3.c expand_quotes.c ft_cd.c \
		ft_echo.c ft_env.c ft_exit.c ft_expand_env_export.c \
		ft_expand_env_export2.c ft_expand.c ft_expand2.c ft_export.c\
		ft_handle_dollar.c ft_here_doc.c ft_itoa.c ft_pwd.c ft_split_tokens.c\
		ft_split .c ft_split_pipes.c ft_strjoin.c ft_strtrim.c ft_unset.c\
		gc.c main.c parse_list.c parsing_utils.c simple_parsing.c\
		syntax_error2.c syntax_error.c utils.c wich_to_expand.c

OBJCS = $(SRCS:.c=.o)

all: $(NAME)

$(NAME) : $(OBJCS)
	$(CC) $(CFLAGS) -o $@ $(OBJCS) -lreadline -lhistory

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean :
	$(RM) $(OBJCS)

fclean : clean
	$(RM) $(NAME)

re: fclean all