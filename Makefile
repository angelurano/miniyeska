NAME = minishell

CC = cc
CFLAGS = -Wall -Wextra -Werror $(INCFLAGS) #-g3

RM = rm -f

INC_DIR = include
INCFLAGS = -I$(INC_DIR) -I$(LIBFT_DIR)
LDFLAGS = -lreadline

SRC_DIR = source
SOURCES = $(SRC_DIR)/main.c \
					$(SRC_DIR)/repl.c \
					$(SRC_DIR)/env.c \
					$(SRC_DIR)/init.c \
					$(SRC_DIR)/signals.c \
					$(SRC_DIR)/errors.c \
					$(SRC_DIR)/errors2.c \
					$(SRC_DIR)/lexer/free.c \
					$(SRC_DIR)/lexer/general.c \
					$(SRC_DIR)/lexer/handlers.c \
					$(SRC_DIR)/lexer/lexer.c \
					$(SRC_DIR)/lexer/segments.c \
					$(SRC_DIR)/lexer/tokens.c  \
					$(SRC_DIR)/parser/command.c \
					$(SRC_DIR)/parser/free.c \
					$(SRC_DIR)/parser/heredoc.c \
					$(SRC_DIR)/parser/heredoc_write.c \
					$(SRC_DIR)/parser/operators.c \
					$(SRC_DIR)/parser/parser.c \
					$(SRC_DIR)/parser/redir.c \
					$(SRC_DIR)/parser/redirs.c \
					$(SRC_DIR)/expansion/alternatives.c \
					$(SRC_DIR)/expansion/atoms.c \
					$(SRC_DIR)/expansion/command.c \
					$(SRC_DIR)/expansion/expansion.c \
					$(SRC_DIR)/expansion/free.c \
					$(SRC_DIR)/expansion/free2.c \
					$(SRC_DIR)/expansion/param.c \
					$(SRC_DIR)/expansion/redirs.c \
					$(SRC_DIR)/expansion/structures.c \
					$(SRC_DIR)/expansion/wildcards.c \
					$(SRC_DIR)/expansion/wildcards2.c \
					$(SRC_DIR)/execute/builtin_exec.c \
					$(SRC_DIR)/execute/cmd_exec.c \
					$(SRC_DIR)/execute/cmd_path.c \
					$(SRC_DIR)/execute/execute.c \
					$(SRC_DIR)/execute/pipe_exec.c \
					$(SRC_DIR)/debug.c

OBJ = $(SOURCES:%.c=%.o)

LIBFT_DIR = libft
LIBFT = $(LIBFT_DIR)/libft.a

all: $(NAME)

$(NAME): $(OBJ) $(LIBFT)
	@echo "Creating miniyeska..."
	@$(CC) $(OBJ) $(CFLAGS) $(LIBFT) $(LDFLAGS) -o $(NAME)
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
