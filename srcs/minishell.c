#include "../incl/minishell.h"

volatile sig_atomic_t	g_signal = 0;

void	shell_init(t_shell *s, char **envp)
{
	struct sigaction	sa;

	ft_bzero(s, sizeof(t_shell));
	s->arenas[0] = arena_new(s, ARENA_SIZE);
	s->arenas[1] = arena_new(s, ARENA_SIZE);
	shell_new_prompt(s);
	s->envp = vector_new(s, 0);
	while (*envp != NULL)
		vector_push(s->envp, string_new(s, *envp++));
	if (isatty(STDIN_FILENO) && isatty(STDOUT_FILENO))
	{
		sa.sa_handler = handle_signals;
		sigemptyset(&sa.sa_mask);
		sa.sa_flags = SA_RESTART;
		if (sigaction(SIGINT, &sa, NULL) == -1)
			shell_exit(s, EXIT_FAILURE, strerror(errno));
		signal(SIGQUIT, SIG_IGN);
	}
}

void	shell_new_prompt(t_shell *s)
{
	size_t			i;
	t_vec *const	old_envp = s->envp;

	s->prompt_count++;
	s->arena = s->arenas[s->prompt_count % 2];
	arena_reset(s->arena);
	if (old_envp != NULL)
	{
		s->envp = vector_new(s, old_envp->capacity);
		i = 0;
		while (i < old_envp->size)
			vector_push(s->envp, string_new(s, old_envp->data[i++]));
	}
	s->cwd = get_working_dir(s);
	g_signal = 0;
}

static char	*shell_readline(t_shell *s, int cont)
{
	char	*input;

	if (cont)
		s->input = readline("> ");
	else
		s->input = readline(get_prompt(s));
	if (s->input == NULL)
	{
		write(1, "exit\n", 5);
		shell_exit(s, EXIT_SUCCESS, NULL);
	}
	input = string_new(s, s->input);
	free(s->input);
	s->input = NULL;
	return (input);
}

static void	user_input(t_shell *s)
{
	char	*input;
	char	*line;
	int		cont;

	input = "";
	cont = 0;
	while (1)
	{
		line = shell_readline(s, cont);
		if (cont && g_signal != SIGINT)
			input = string_join(s, input, string_join(s, " ", line));
		else
			input = line;
		s->tokens = tokenize(s, input);
		if (s->tokens == NULL)
			add_history(input);
		if (s->tokens == NULL || s->tokens->size == 0)
			break ;
		cont = strcmp(s->tokens->data[s->tokens->size - 1], "|") == 0;
		if (cont)
			continue ;
		add_history(input);
		return (shell_execute(s));
	}
}

int	main(int argc, char **argv, char **envp)
{
	t_shell	shell;

	(void)argc;
	(void)argv;
	shell_init(&shell, envp);
	while (1)
	{
		shell_new_prompt(&shell);
		user_input(&shell);
	}
	shell_exit(&shell, 0, NULL);
}
