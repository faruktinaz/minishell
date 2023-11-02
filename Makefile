SRCS = ./srcs/executor/main.c \
		./srcs/heredoc.c \
		./srcs/executor/built_in.c \
		./srcs/executor/built_in_2.c \
		./srcs/executor/utils_built_in.c \
		./srcs/executor/utils_built_in_2.c \
		./srcs/executor/signals.c \
		./srcs/executor/pipes.c \
		./srcs/executor/readline.c \
		./srcs/redirections.c \
		./srcs/parser/parser_process.c \
		./srcs/parser/parser_process2.c \
		./srcs/parser/changed_list.c \
		./srcs/parser/utils.c \
		./srcs/parser/utilslibft.c \
		./srcs/parser/error_check.c \
		./srcs/parser/env_check.c \
		./srcs/parser/env_check2.c \
		./srcs/parser/trim_quot.c \
		./srcs/lexer/make_sense.c
NAME = minishell
CC = gcc
CFLAGS = -Wall -Wextra -Werror -lreadline -g
RM = rm -rf
LIBFT = ./srcs/libary/libft/libft.a
OBJS = $(SRCS:.c=.o)
READLINE = readline

all: $(READLINE) $(NAME)

$(NAME): $(SRCS) $(LIBFT)
	@gcc $(CFLAGS) $(SRCS) $(LIBFT) -L${PWD}/readline/lib  -I${PWD}/readline/include/ -o $(NAME)
	@echo "************************"
	@echo "   MINISHELL CREATED"
	@echo "************************"

$(READLINE):
	curl -O https://ftp.gnu.org/gnu/readline/readline-8.2.tar.gz
	tar -xvf readline-8.2.tar.gz
	cd readline-8.2 && ./configure --prefix=${PWD}/readline
	cd readline-8.2 && make install

$(LIBFT) :
	@make -C ./srcs/libary/libft
	@make bonus -C ./srcs/libary/libft

clean:
	$(RM) $(OBJS)
	@make fclean -C ./srcs/libary/libft

fclean: clean
	$(RM) $(NAME)
	@make fclean -C ./srcs/libary/libft

re: fclean all

.PHONY: all bonus clean fclean re .c.om