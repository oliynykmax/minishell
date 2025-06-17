#include "../incl/minishell.h"

/* readline event hook for main shell interface
 * handles Ctrl+C interruption during normal input
 * shows ^C and exits readline cleanly
 */
int	my_rl_event_hook(void)
{
	static int	printed = 0;

	if (g_signal == SIGINT)
	{
		if (!printed)
		{
			printed = 1;
			write(STDOUT_FILENO, "^C\n", 3);
		}
		rl_replace_line("", 0);
		rl_on_new_line();
		rl_done = 1;
	}
	else
		printed = 0;
	return (0);
}

/* readline event hook for heredoc input
 * handles Ctrl+C interruption during heredoc
 * shows ^C without extra newline to prevent prompt formatting issues
 */
int	heredoc_event_hook(void)
{
	static int	printed = 0;

	if (g_signal == SIGINT)
	{
		if (!printed)
		{
			printed = 1;
			write(STDOUT_FILENO, "^C", 2);
		}
		rl_replace_line("", 0);
		rl_on_new_line();
		rl_done = 1;
		return (0);
	}
	else
		printed = 0;
	return (0);
}
