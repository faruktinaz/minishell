SRCS = ./srcs/main.c \
		./srcs/parser/parser_process.c \
		./srcs/parser/parser_process2.c \
		./srcs/parser/utils.c \
		./srcs/parser/utilslibft.c \
		./srcs/parser/error_check.c \
		./srcs/parser/env_check.c \
		./srcs/parser/trim_quot.c \
		./srcs/lexer/make_sense.c
NAME = minishell
CC = gcc
CFLAGS = -Wall -Wextra -Werror -lreadline -g
RM = rm -rf
LIBFT = ./srcs/libary/libft/libft.a
OBJS = $(SRCS:.c=.o)

all: $(NAME)

$(NAME): $(SRCS) $(LIBFT)
	@gcc $(CFLAGS) $(SRCS) $(LIBFT) -o $(NAME)
	@echo "************************"
	@echo "   MINISHELL CREATED"
	@echo "************************"

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