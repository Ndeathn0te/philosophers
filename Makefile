NAME = philo
CC = clang
CFLAGS = -Wall -Wextra -Werror -pthread
LIBFT_DIR = ./libft
LIBFT_A = $(LIBFT_DIR)/libft.a
INCLUDES = -I. -I$(LIBFT_DIR)

SRCS = main.c init.c monitor.c utils.c error.c routine.c
OBJS = $(SRCS:.c=.o)

all: $(NAME)

$(NAME): $(OBJS) $(LIBFT_A)
	$(CC) $(CFLAGS) $(INCLUDES) $(OBJS) $(LIBFT_A) -o $(NAME)

$(LIBFT_A):
	@echo "Compilando libft..."
	make -C $(LIBFT_DIR)

clean:
	make clean -C $(LIBFT_DIR)
	rm -f $(OBJS)

fclean: clean
	make fclean -C $(LIBFT_DIR)
	rm -f $(NAME)

re: fclean all