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
	if (mode == INPUT_MAIN)
	{
		write(1, "exit\n", 5);
		shell_exit(s, EXIT_SUCCESS, NULL);
	}
	else if (mode == INPUT_NONINTERACTIVE)
	{
		shell_exit(s, s->last_status, NULL);
	}
	return (string_new(s, ""));
}

static char	*read_noninteractive_line(t_shell *s)
{
	char	buffer[4096];
	int		bytes_read;
	int		i;

	i = 0;
	while (i < 4095)
	{
		bytes_read = read(STDIN_FILENO, &buffer[i], 1);
		if (bytes_read <= 0)
		{
			if (i == 0)
				return (NULL);
			break ;
		}
		if (buffer[i] == '\n')
			break ;
		i++;
	}
	buffer[i] = '\0';
	return (string_new(s, buffer));
}

char	*shell_readline(t_shell *s, t_input_mode mode)
{
	char	*input;

	if (s->input_mode == INPUT_NONINTERACTIVE)
	{
		input = read_noninteractive_line(s);
		if (input == NULL)
			return (handle_null_input(s, INPUT_NONINTERACTIVE));
		return (input);
	}
	s->input_mode = mode;
	setup_signals(0);
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
