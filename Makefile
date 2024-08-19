SRCS= \
	program/ft_parsing.c program/ft_traceroute.c \
	program/ft_error_and_exit.c \
	program/ft_request.c program/ft_getting_src_addr.c \
	program/ft_utils_libft.c program/ft_debogage.c

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