#include "../incl/minishell.h"

char	*get_prompt(t_shell *s)
{
	char	*prompt;

	prompt = string_join(s, ANSI_COLOR_GREEN, get_working_dir(s));
	prompt = string_join(s, prompt, ANSI_COLOR_RESET MINISHELL_PROMPT);
	return (prompt);
}

// Clean up all resources and exit the shell, with an optional error message.
void	shell_exit(t_shell *s, int exit_status, const char *message)
{
	if (message != NULL)
		ft_fprintf(2, "minishell: %s\n", message);
	safe_close(&s->fd_in);
	safe_close(&s->fd_out);
	safe_close(&s->fd_unused);
	safe_close(&s->fd_saved_in);
	safe_close(&s->fd_saved_out);
	if (s->dirent != NULL)
		closedir(s->dirent);
	free(s->input);
	arena_free(s->arenas[0]);
	arena_free(s->arenas[1]);
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
		{
			if (s->cwd)
				return (s->cwd);
			else
				return ("/");
		}
		size *= 2;
	}
}

int	is_blank(char c)
{
	return (c == ' ' || c == '\t' || c == '\n');
}

int	is_meta(char c)
{
	return (c == '|' || c == '<' || c == '>');
}
