#include "minishell.h"

// Given a command (like "ls"), get the program filename for that command (like
// "/usr/bin/ls"). If the command contains a slash, it's returned directly.
// Otherwise, the directories listed in the PATH environment variable are
// searched, and the first executable file matching the name of the command is
// returned. Returns NULL if the command could not be found.

// Helper: Search PATH for the command
static char	*search_path_for_command(t_shell *s, char *command, char *path)
{
	char	*search;
	char	*delimiter;
	char	*directory;
	char	*filename;

	search = string_join(s, "/", command);
	while (*path)
	{
		delimiter = ft_strchr(path, ':');
		if (!delimiter)
			delimiter = ft_strchr(path, '\0');
		directory = string_sub(s, path, delimiter - path);
		filename = string_join(s, directory, search);
		if (access(filename, F_OK | X_OK) == 0)
			return (filename);
		path = delimiter + (*delimiter == ':');
	}
	return (NULL);
}

static char	*get_command_filename(t_shell *s, char *command)
{
	char	*path;

	if (ft_strchr(command, '/'))
		return (command);
	path = get_env_variable(s, "PATH");
	if (!path || !*path)
	{
		if (access(command, F_OK) == 0)
			return (command);
		return (NULL);
	}
	return (search_path_for_command(s, command, path));
}

void	subprocess_run(t_shell *s, t_vec *command)
{
	char *const	name = command->data[0];
	char *const	filename = get_command_filename(s, name);
	struct stat	st;

	if (ft_strcmp(name, ".") == 0 || ft_strcmp(name, "..") == 0
		|| name[0] == '\0')
		exit_with_msg(s, name, ": command not found", 127);
	if (filename == NULL)
		handle_not_found(s, name);
	if (stat(filename, &st) == 0)
		handle_stat_cases(s, filename, &st);
	else
		handle_not_found(s, name);
	execve(filename, (char **)command->data, (char **)s->envp->data);
	shell_exit(s, 126, strerror(errno));
}
