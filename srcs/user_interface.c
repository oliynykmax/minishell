#include "../incl/minishell.h"

char	*reprompt(t_shell *s)
{
	s->last_status = 130;
	g_signal = 0;
	if (s->input)
	{
		free(s->input);
		s->input = NULL;
	}
	return (string_new(s, ""));
}

static char	*handle_null_input(t_shell *s, t_input_mode mode)
{
	if (mode == INPUT_MAIN && isatty(STDIN_FILENO))
	{
		write(1, "exit\n", 5);
		shell_exit(s, EXIT_SUCCESS, NULL);
	}
	return (string_new(s, ""));
}

char	*shell_readline(t_shell *s, t_input_mode mode)
{
	char	*input;

	s->input_mode = mode;
	if (mode == INPUT_HEREDOC)
		setup_heredoc_signals();
	else
		setup_parent_signals();
	if (mode == INPUT_PIPE)
		s->input = readline("> ");
	else
		s->input = readline(get_prompt(s));
	if (g_signal == SIGINT)
		return (reprompt(s));
	if (s->input == NULL)
		return (handle_null_input(s, mode));
	input = string_new(s, s->input);
	free(s->input);
	s->input = NULL;
	return (input);
}

void	user_input(t_shell *s)
{
	while (1)
	{
		process_pipe_input(s);
		if (g_signal == SIGINT)
		{
			s->last_status = 130;
			g_signal = 0;
			continue ;
		}
		if (s->tokens == NULL || s->tokens->size == 0)
			continue ;
		shell_execute(s, (char **) s->tokens->data);
		break ;
	}
}
