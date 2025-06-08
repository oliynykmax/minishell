#include "../incl/minishell.h"

void	handle_signals(int sig)
{
	if (sig == SIGINT)
	{
		g_signal = 1;
		printf("\n");
		rl_replace_line("", 0);
		rl_on_new_line();
		rl_redisplay();
	}
}
