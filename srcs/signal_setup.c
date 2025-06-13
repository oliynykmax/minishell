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

void	handle_pending_signals(t_shell *s)
{
	if (g_signal == SIGINT)
	{
		s->last_status = 130;
		if (s->input_mode == INPUT_HEREDOC)
			handle_heredoc_signals();
		else if (s->input_mode == INPUT_PIPE)
		{
			write(STDOUT_FILENO, "\n", 1);
			g_signal = 0;
		}
		else
			g_signal = 0;
	}
}
