#include "../incl/minishell.h"
#include <errno.h>
#include <linux/limits.h>
#include <stddef.h>

volatile sig_atomic_t	g_signal = 0;

static void	print_tokens(t_vec *tokens)
{
	size_t	i;

	i = 0;
	while (i < tokens->size)
	{
		printf("[%s] ", (char *)tokens->data[i]);
		i++;
	}
	printf("\n");
}

void	shell_init(t_shell *s, char **envp)
{
	ft_bzero(s, sizeof(t_shell));
	s->arenas[0] = arena_new(s, ARENA_SIZE);
	s->arenas[1] = arena_new(s, ARENA_SIZE);
	shell_new_prompt(s);
	s->envp = vector_new(s, 0);
	while (*envp != NULL)
		vector_push(s->envp, string_new(s, *envp++));
	s->sa.sa_handler = handle_signals;
	sigemptyset(&s->sa.sa_mask);
	s->sa.sa_flags = SA_RESTART;
	if (sigaction(SIGINT, &s->sa, NULL) == -1)
		shell_exit(s, EXIT_FAILURE, strerror(errno));
	signal(SIGQUIT, SIG_IGN);
}

// Clean up all resources and exit the shell, with an optional error message.

void	shell_exit(t_shell *s, int exit_status, const char *message)
{
	free(s->input);
	arena_free(s->arenas[0]);
	arena_free(s->arenas[1]);
	if (message != NULL)
		printf("minishell: %s\n", message);
	exit(exit_status);
}

static char	*get_working_dir(t_shell *s)
{
	size_t	size;
	char	*path;

	size = PATH_MAX;
	while (1)
	{
		path = shell_malloc(s, size);
		if (getcwd(path, size) != NULL)
			return (path);
		if (errno != ERANGE)
			shell_exit(s, EXIT_FAILURE, strerror(errno));
		size *= 2;
	}
	return (path);
}

// Begin a new prompt, starting over with a new, empty memory arena. Environment
// variables are copied into the new arena so that they are preserved across
// prompts.

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

static void	shell_loop(t_shell *s)
{
	while (1)
	{
		shell_new_prompt(s);
		printf("\n%s", s->cwd);
		s->input = readline("\n🐚> ");
		if (s->input == NULL)
			shell_exit(s, EXIT_SUCCESS, NULL);
		s->tokens = tokenize(s, s->input);
		if (s->tokens->size > 0)
			add_history(s->input);
		free(s->input);
		s->input = NULL;
		if (g_signal || s->tokens->size == 0)
			continue ;
		print_tokens(s->tokens);
		if (ft_strcmp(s->tokens->data[0], "exit") == 0)
			shell_exit(s, EXIT_SUCCESS, NULL);
	}
}

int	main(int argc, char **argv, char **envp)
{
	t_shell	shell;

	(void)argc;
	(void)argv;
	shell_init(&shell, envp);
	shell_loop(&shell);
	rl_clear_history();
	shell_exit(&shell, 0, NULL);
}
