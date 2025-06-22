
NAME    := minishell

CC      := cc
CFLAGS  := -Wall -Wextra -Werror

INCLUDES := -Ilibft

SRCS    := \
 	src/builtin_echo.c \
 	src/builtin_cd.c \
 	src/builtin_pwd.c \
 	src/builtin_export.c \
 	src/builtin_unset.c \
 	src/builtin_env.c \
 	src/builtin_exit.c \
 	src/bn_helpers.c \
 	src/command_exec.c \
 	src/execution.c \
 	src/filename.c \
 	src/heredoc.c \
 	src/heredoc_utils.c \
 	src/pipe_extra_input.c \
 	src/memory.c \
 	src/minishell.c \
 	src/params.c \
 	src/redirect.c \
 	src/shell_exec.c \
 	src/shell_fork_utils.c \
 	src/signals.c \
 	src/split_words.c \
 	src/string.c \
 	src/subprocess.c \
 	src/temp_files.c \
 	src/tokenize.c \
 	src/user_interface.c \
 	src/utils.c \
 	src/vector.c \

OBJS    := $(SRCS:src/%.c=obj/%.o)

LIBFT_DIR := libft
LIBFT_A   := $(LIBFT_DIR)/libft.a

.PHONY: all clean fclean re
.SECONDARY : $(OBJS)
.SILENT : obj

all: $(NAME)
	@echo "🎉 Build complete!"

$(NAME): $(OBJS) $(LIBFT_A)
	@echo "🔗 Linking $(NAME)..."
	@$(CC) $(CFLAGS) $(OBJS) -lreadline $(LIBFT_A) -o $(NAME)

obj/%.o: src/%.c src/minishell.h | obj
	@$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

$(LIBFT_A):
	@echo "📚 Building libft..."
	@$(MAKE) --no-print-directory -C $(LIBFT_DIR)

obj:
	@mkdir -p obj

clean:
	@echo "🧹 Cleaning..."
	@rm -rf obj
	@$(MAKE) --no-print-directory -C $(LIBFT_DIR) clean

fclean: clean
	@echo "🗑️  Removing $(NAME)"
	@rm -f $(NAME)
	@$(MAKE) --no-print-directory -C $(LIBFT_DIR) fclean

re:
	@echo "🔄 Rebuilding..."
	@$(MAKE) --no-print-directory fclean
	@$(MAKE) --no-print-directory all
