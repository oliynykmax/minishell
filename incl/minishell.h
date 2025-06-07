#ifndef MINISHELL_H
# define MINISHELL_H

# pragma once
# include "../libft/libft.h"
# include "tokenize.h"
# include <dirent.h>
# include <fcntl.h>
# include <readline/history.h>
# include <readline/readline.h>
# include <signal.h>
# include <stdatomic.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <sys/ioctl.h>
# include <sys/stat.h>
# include <sys/types.h>
# include <sys/wait.h>
# include <term.h>
# include <termios.h>
# include <unistd.h>

typedef struct shell
{
	char						*input;
	t_token						*tokens;
	struct sigaction			sa;
}								t_shell;

typedef enum e_shell_status
{
	SHELL_CONTINUE,
	SHELL_EXIT
}								t_sstatus;

extern volatile sig_atomic_t	g_signal;

void							handle_signals(int signum);

#endif
