
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
	srcs/params.c \
	srcs/pwd.c \
	srcs/signals.c \
	srcs/string.c \
	srcs/token_types.c \
	srcs/tokenize.c \
	srcs/utils.c \
	srcs/vector.c \
	srcs/debug.c
OBJS    := $(SRCS:srcs/%.c=objs/%.o)

LIBFT_DIR := libft
LIBFT_A   := $(LIBFT_DIR)/libft.a

.PHONY: all clean fclean re norm
.SECONDARY : $(OBJS)
.SILENT : objs

all: $(NAME)
	@echo "🎉 Build complete!"

$(NAME): $(OBJS) $(LIBFT_A)
	@echo "🔗 Linking $(NAME)..."
	@$(CC) $(CFLAGS) $(OBJS) -lreadline -ltinfo $(LIBFT_A) -o $(NAME)

objs/%.o: srcs/%.c incl/minishell.h | objs
	@$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

$(LIBFT_A):
	@echo "📚 Building libft..."
	@$(MAKE) --no-print-directory -C $(LIBFT_DIR)

objs:
	@mkdir -p objs



clean:
	@echo "🧹 Cleaning..."
	@rm -rf objs
	@$(MAKE) --no-print-directory -C $(LIBFT_DIR) clean

fclean: clean
	@echo "🗑️  Removing $(NAME)"
	@rm -f $(NAME)
	@$(MAKE) --no-print-directory -C $(LIBFT_DIR) fclean

re:
	@echo "🔄 Rebuilding..."
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
