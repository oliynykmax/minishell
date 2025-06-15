#include "../incl/minishell.h"

volatile sig_atomic_t	g_signal;

/* general signal handling */
void	handle_signals(int sig)
{
	g_signal = sig;
}

int	my_rl_event_hook(void)
{
	static int	printed = 0;

	if (g_signal == SIGINT)
	{
		if (!printed)
		{
			printed = 1;
			write(1, "^C\n", 3);
		}
		rl_replace_line("", 0);
		rl_on_new_line();
		rl_done = 1;
	}
	else
		printed = 0;
	return (0);
}

void	setup_parent_signals(void)
{
	struct sigaction	sa;

	sa.sa_handler = handle_signals;
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = 0;
	sigaction(SIGINT, &sa, NULL);
	signal(SIGQUIT, SIG_IGN);
	rl_event_hook = my_rl_event_hook;
	rl_catch_signals = 0;
}

void	setup_child_signals(void)
{
	struct sigaction	sa;

	sa.sa_handler = SIG_DFL;
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = 0;
	sigaction(SIGINT, &sa, NULL);
	sigaction(SIGQUIT, &sa, NULL);
}

void	ignore_sigpipe(void)
{
	struct sigaction	sa;

	sa.sa_handler = SIG_IGN;
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = 0;
	sigaction(SIGPIPE, &sa, NULL);
}
