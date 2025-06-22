#include "minishell.h"

volatile sig_atomic_t	g_signal;

/* general signal handling */
void	handle_signals(int sig)
{
	g_signal = sig;
	rl_done = 1;
}

/* ignore SIGPIPE signal for pipe operations */
void	ignore_sigpipe(void)
{
	struct sigaction	sa;

	sa.sa_handler = SIG_IGN;
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = 0;
	sigaction(SIGPIPE, &sa, NULL);
}

/* readline event hook for main shell interface
 * handles Ctrl+C interruption during normal input
 * shows ^C and exits readline cleanly
 */
int	my_rl_event_hook(void)
{
	return (0);
}

/* unified signal setup function
 * is_child: true for child process (default handlers),
 * false for parent (custom handlers)
 */
void	setup_signals(int is_child)
{
	struct sigaction	sa;

	sigemptyset(&sa.sa_mask);
	sa.sa_flags = 0;
	if (is_child)
	{
		sa.sa_handler = SIG_DFL;
		sigaction(SIGINT, &sa, NULL);
		sigaction(SIGQUIT, &sa, NULL);
	}
	else
	{
		sa.sa_handler = handle_signals;
		sigaction(SIGINT, &sa, NULL);
		signal(SIGQUIT, SIG_IGN);
		rl_event_hook = my_rl_event_hook;
	}
}
