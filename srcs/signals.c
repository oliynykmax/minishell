#include "../incl/minishell.h"

volatile sig_atomic_t	g_signal;

/* helper for setting up sigaction structure */
static void	setup_sigaction(struct sigaction *sa, void (*handler)(int))
{
	sa->sa_handler = handler;
	sigemptyset(&sa->sa_mask);
	sa->sa_flags = 0;
}

/* general signal handling */
void	handle_signals(int sig)
{
	g_signal = sig;
}

/* centralized signal setup helper */
void	setup_signal_base(void (*handler)(int), int (*event_hook)(void))
{
	struct sigaction	sa;

	setup_sigaction(&sa, handler);
	sigaction(SIGINT, &sa, NULL);
	signal(SIGQUIT, SIG_IGN);
	rl_event_hook = event_hook;
}

/* setup signals for parent process (main shell) */
void	setup_parent_signals(void)
{
	setup_signal_base(handle_signals, my_rl_event_hook);
	rl_catch_signals = 0;
}

/* ignore SIGPIPE signal for pipe operations */
void	ignore_sigpipe(void)
{
	struct sigaction	sa;

	setup_sigaction(&sa, SIG_IGN);
	sigaction(SIGPIPE, &sa, NULL);
}
