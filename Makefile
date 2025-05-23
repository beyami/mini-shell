NAME = minishell

CC = cc
CFLAGS = -Wall -Wextra -Werror

RM = rm -f

LIB_DIR = ./lib
SRCS_DIR = ./srcs


LIB = $(LIB_DIR)/lib.a -lreadline

SRCS = $(wildcard $(SRCS_DIR)/*.c)
OBJS = $(SRCS:.c=.o)
INCLUDES = -I ./includes -I ./$(LIB_DIR)/includes

all: $(NAME)

$(NAME): $(OBJS)
	$(MAKE) -C $(LIB_DIR)
	$(CC) $(CFLAGS) $(OBJS) $(LIB) -o $(NAME)

.c.o:
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

clean:
	$(MAKE) -C $(LIB_DIR) clean
	$(RM) $(OBJS)

fclean: clean
	$(MAKE) -C $(LIB_DIR) fclean
	$(RM) $(NAME)

re: fclean all

.PHONY: all clean fclean re