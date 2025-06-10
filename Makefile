
# Colors
RESET	= \033[0m
GREEN	= \033[32m
YELLOW	= \033[33m

NAME    := minishell

CC      := cc
CFLAGS  := -Wall -Wextra -Werror

INCLUDES := -Iincl -Ilibft

SRCS    := \
	srcs/env.c \
	srcs/execution.c \
	srcs/exit.c \
	srcs/export.c \
	srcs/memory.c \
	srcs/minishell.c \
	srcs/pwd.c \
	srcs/signals.c \
	srcs/string.c \
	srcs/token_types.c \
	srcs/tokenize.c \
	srcs/utils.c \
	srcs/vector.c

ECHO := echo -e

OBJS    := $(SRCS:srcs/%.c=objs/%.o)

LIBFT_DIR := libft
LIBFT_A   := $(LIBFT_DIR)/libft.a

.PHONY: all clean fclean re norm
.SECONDARY : $(OBJS)
.SILENT : objs

all: $(NAME)
	@echo "$(GREEN)🎉 Build complete!$(RESET)"

$(NAME): $(OBJS) $(LIBFT_A)
	@echo "$(YELLOW)🔗 Linking $(NAME)...$(RESET)"
	@$(CC) $(CFLAGS) $(OBJS) -lreadline -ltinfo $(LIBFT_A) -o $(NAME)

objs/%.o: srcs/%.c incl/minishell.h | objs
	@$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

$(LIBFT_A):
	@echo "$(GREEN)📚 Building libft...$(RESET)"
	@$(MAKE) --no-print-directory -C $(LIBFT_DIR)

objs:
	@mkdir -p objs



clean:
	@echo "$(YELLOW)🧹 Cleaning...$(RESET)"
	@rm -rf objs
	@$(MAKE) --no-print-directory -C $(LIBFT_DIR) clean

fclean: clean
	@echo "$(YELLOW)🗑️  Removing $(NAME)$(RESET)"
	@rm -f $(NAME)
	@$(MAKE) --no-print-directory -C $(LIBFT_DIR) fclean

re:
	@echo "$(GREEN)🔄 Rebuilding...$(RESET)"
	@$(MAKE) --no-print-directory fclean
	@$(MAKE) --no-print-directory all

norm:
	@norminette | awk '\
		/^.*: Error!/ { file = $$1; seen = 0; next } \
		/Error:/ && $$0 !~ /INVALID_HEADER/ { \
			if (!seen) { print file ": Error!"; seen = 1 } \
			print \
		} \
	'

valgrind:
	@valgrind --leak-check=full --show-leak-kinds=all --suppressions=mini.supp ./minishell
