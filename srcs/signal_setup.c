#include "../incl/minishell.h"

/* setup signals for child processes */
void	setup_child_signals(void)
{
	struct sigaction	sa;

	sa.sa_handler = SIG_DFL;
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = 0;
	sigaction(SIGINT, &sa, NULL);
	sigaction(SIGQUIT, &sa, NULL);
}

/* setup signals for heredoc with custom event hook */
void	setup_heredoc_signals_local(void)
{
	setup_signal_base(handle_signals, heredoc_event_hook);
}
