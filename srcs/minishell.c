#include "../incl/minishell.h"

void	shell_init(t_shell *s, char **envp)
{
	ft_bzero(s, sizeof(t_shell));
	s->arenas[0] = arena_new(s, ARENA_SIZE);
	s->arenas[1] = arena_new(s, ARENA_SIZE);
	shell_new_prompt(s);
	s->envp = vector_new(s, 0);
	while (*envp != NULL)
		vector_push(s->envp, string_new(s, *envp++));
	s->input_mode = INPUT_MAIN;
	if (isatty(STDIN_FILENO) && isatty(STDOUT_FILENO))
		setup_parent_signals();
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

int	main(int argc, char **argv, char **envp)
{
	t_shell	shell;

	(void)argc;
	(void)argv;
	shell_init(&shell, envp);
	if (argc == 3 && ft_strcmp(argv[1], "-c") == 0)
		debug_mode(&shell, argv[2], envp);
	while (1)
	{
		shell_new_prompt(&shell);
		user_input(&shell);
	}
	shell_exit(&shell, 0, NULL);
}
