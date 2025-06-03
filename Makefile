NAME = minishell

CC = cc
# TODO: -gあとで消す
CFLAGS = -Wall -Wextra -Werror -g

RM = rm -f

LIB_DIR = ./lib
# TODO: debugあとで消す
SRCS_DIR = ./srcs ./srcs/builtin ./srcs/execution ./srcs/tokenization ./srcs/init ./srcs/expansion ./srcs/utils ./srcs/parsing ./srcs/redirection ./srcs/signal ./debug ./srcs/env

LIB = $(LIB_DIR)/lib.a -lreadline

# TODO: wildcard削除
SRCS = $(foreach dir,$(SRCS_DIR),$(wildcard $(dir)/*.c))
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
