NAME    := minishell

CC      := cc
CFLAGS  := -Wall -Wextra -Werror

INCLUDES := -Iincl -Ilibft

SRCS    := \
	srcs/memory.c \
	srcs/minishell.c \
	srcs/signals.c \
	srcs/string.c \
	srcs/tokenize.c \
	srcs/vector.c \

OBJS    := $(SRCS:srcs/%.c=objs/%.o)

LIBFT_DIR := libft
LIBFT_A   := $(LIBFT_DIR)/libft.a

.PHONY: all clean fclean re norm
.SECONDARY : $(OBJS)
all: $(NAME)

$(NAME): $(OBJS) $(LIBFT_A)
	$(CC) $(CFLAGS) $(OBJS) -lreadline -ltinfo $(LIBFT_A) -o $(NAME)

objs/%.o: srcs/%.c incl/minishell.h | objs
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

$(LIBFT_A):
	@$(MAKE) --no-print-directory -C $(LIBFT_DIR)
	@echo "make libft"

objs:
	@mkdir -p objs
clean:
	rm -rf objs
	@$(MAKE) --no-print-directory -C $(LIBFT_DIR) clean
	@echo "clean libft"

fclean: clean
	rm -f $(NAME)
	@$(MAKE) --no-print-directory -C $(LIBFT_DIR) fclean
	@echo "fclean libft"

re:
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
	valgrind --leak-check=full --show-leak-kinds=all --suppressions=mini.supp ./minishell
