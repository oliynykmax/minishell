#include "../incl/minishell.h"

void	setup_heredoc_signals(void)
{
	struct sigaction	sa;

	sa.sa_handler = handle_signals;
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = 0;
	sigaction(SIGINT, &sa, NULL);
	signal(SIGQUIT, SIG_IGN);
	rl_event_hook = NULL;
}

void	handle_heredoc_signals(void)
{
	if (g_signal == SIGINT)
	{
		g_signal = 0;
		rl_done = 1;
	}
}

int	heredoc_event_hook(void)
{
	if (g_signal == SIGINT)
	{
		write(STDOUT_FILENO, "^C\n", 3);
		rl_replace_line("", 0);
		rl_on_new_line();
		rl_done = 1;
		return (0);
	}
	return (0);
}

void	setup_heredoc_signals_local(void)
{
	struct sigaction	sa;

	sa.sa_handler = handle_signals;
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = 0;
	sigaction(SIGINT, &sa, NULL);
	signal(SIGQUIT, SIG_IGN);
	rl_event_hook = heredoc_event_hook;
}
