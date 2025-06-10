#include "../incl/minishell.h"

char	*get_prompt(t_shell *s)
{
	char	*prompt;
	char	*current_dir;

	current_dir = get_working_dir(s);
	prompt = string_join(s, STR_PROMPTSTART, BGRN);
	prompt = string_join(s, prompt, current_dir);
	prompt = string_join(s, prompt, CRESET);
	prompt = string_join(s, prompt, STR_PROMPTDELIM);
	return (prompt);
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

char	*get_working_dir(t_shell *s)
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
