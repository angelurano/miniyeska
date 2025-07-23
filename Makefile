NAME = minishell

CC = cc
CFLAGS = -Wall -Wextra -Werror $(INCLUDES)

RM = rm -f

INC_DIR = include
INCLUDES = -I$(INC_DIR) -I$(LIBFT_DIR) -lreadline

SRC_DIR = source
SOURCES = source/main.c
OBJ = $(SOURCES:.c=.o)

LIBFT_DIR = libft
LIBFT = $(LIBFT_DIR)/libft.a

all: $(NAME)

$(NAME): $(OBJ) $(LIBFT)
	@echo "Creating miniyeska..."
	@$(CC) $(OBJ) $(CFLAGS) $(LIBFT) -o $(NAME)
	@echo "Finish!"

$(LIBFT):
	@echo "Compiling libft..."
	@make -C $(LIBFT_DIR) --no-print-directory

%.o: %.c
	@$(CC) $(CFLAGS) -c $< -o $@

clean:
	make -C $(LIBFT_DIR) fclean --no-print-directory
	$(RM) $(OBJ)

fclean: clean
	$(RM) $(NAME)

re: fclean all

dev: CFLAGS += -g3 -fsanitize=address,leak,undefined
dev: all

.PHONY: all clean fclean re
