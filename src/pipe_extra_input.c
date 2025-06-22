#include "minishell.h"

static char	*read_multiline_pipeline(t_shell *s, char *input)
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
	if (s->input_mode != INPUT_NONINTERACTIVE && ft_strcmp(input, ""))
		add_history(input);
	if (s->tokens == NULL || s->tokens->size == 0)
		return (input);
	input = read_multiline_pipeline(s, input);
	return (input);
}
