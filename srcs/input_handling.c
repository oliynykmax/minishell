#include "../incl/minishell.h"

static char	*handle_pipe_continuation(t_shell *s, char *input)
{
	char	*line;
	int		cont;

	cont = ft_strcmp(s->tokens->data[s->tokens->size - 1], "|") == 0;
	while (cont)
	{
		line = shell_readline(s, INPUT_PIPE);
		if (g_signal == SIGINT)
		{
			s->last_status = 130;
			return (string_new(s, ""));
		}
		if (line && line[0] == '\0')
			break ;
		input = string_join(s, input, string_join(s, " ", line));
		s->tokens = tokenize(s, input);
		if (s->tokens == NULL || s->tokens->size == 0)
			break ;
		cont = ft_strcmp(s->tokens->data[s->tokens->size - 1], "|") == 0;
	}
	return (input);
}

char	*process_pipe_input(t_shell *s)
{
	char	*input;

	input = shell_readline(s, INPUT_MAIN);
	if (g_signal == SIGINT)
	{
		s->last_status = 130;
		return (string_new(s, ""));
	}
	s->tokens = tokenize(s, input);
	if (ft_strcmp(input, ""))
		add_history(input);
	if (s->tokens == NULL || s->tokens->size == 0)
		return (input);
	input = handle_pipe_continuation(s, input);
	return (input);
}
