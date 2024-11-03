SRCS= \
	program/main.c program/libft.c program/parser.c \
	program/exit.c program/utils.c

OBJS = $(SRCS:.c=.o)

GCC = gcc -Wall -Wextra -Werror

NAME = ft_traceroute

all: $(NAME)

$(NAME): $(OBJS)
	$(GCC) -o $(NAME) $(OBJS)

.c.o:
	$(GCC) -c $< -o $@

clean:
	rm -f $(OBJS)

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: re clean fclean all