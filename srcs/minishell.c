#include "../incl/minishell.h"

void	shell_init(t_shell *s, char **envp)
{
	ft_bzero(s, sizeof(t_shell));
	s->fd_in = -1;
	s->fd_out = -1;
	s->fd_unused = -1;
	s->arenas[0] = arena_new(s, ARENA_SIZE);
	s->arenas[1] = arena_new(s, ARENA_SIZE);
	shell_new_prompt(s);
	s->envp = vector_new(s, 0);
	while (*envp != NULL)
		vector_push(s->envp, string_new(s, *envp++));
	if (isatty(STDIN_FILENO) && isatty(STDOUT_FILENO))
	{
		s->input_mode = INPUT_MAIN;
		setup_signals(0);
	}
	else
		s->input_mode = INPUT_NONINTERACTIVE;
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
	s->cwd = string_new(s, s->cwd);
	s->cwd = get_working_dir(s);
	g_signal = 0;
}

int	main(int argc, char **argv, char **envp)
{
	t_shell	shell;

	shell_init(&shell, envp);
	if (argc == 3 && ft_strcmp(argv[1], "-c") == 0)
	{
		shell.tokens = tokenize(&shell, argv[2]);
		shell_execute(&shell, (char **) shell.tokens->data);
		shell_exit(&shell, 0, NULL);
	}
	while (1)
	{
		shell_new_prompt(&shell);
		user_input(&shell);
	}
}
