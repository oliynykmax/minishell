#ifndef MINISHELL_H
# define MINISHELL_H

#pragma once
# include "../libft/libft.h"
# include <dirent.h>
# include <fcntl.h>
# include <readline/history.h>
# include <readline/readline.h>
# include <signal.h>
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
# include <stdatomic.h>

extern volatile sig_atomic_t	g_signal;

void	handle_signals(int signum);

#endif
