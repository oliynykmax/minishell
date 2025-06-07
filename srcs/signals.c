#include "../incl/minishell.h"

void	handle_signals(int signum)
{
	if (signum == SIGINT)
	{
		g_signal = 1;
		// safe_write_string("\n");
	}
}
