#include "../incl/minishell.h"

// Given a command (like "ls"), get the program filename for that command (like
// "/usr/bin/ls"). If the command contains a slash, it's returned directly.
// Otherwise, the directories listed in the PATH environment variable are
// searched, and the first executable file matching the name of the command is
// returned. Returns NULL if the command could not be found.

static char	*get_command_filename(t_shell *s, char *command)
{
	char	*path;
	char	*delimiter;
	char	*directory;
	char	*filename;

	if (ft_strchr(command, '/') != NULL)
		return (command);
	path = get_env_variable(s, "PATH");
	command = string_join(s, "/", command);
	while (*path != '\0')
	{
		delimiter = ft_strchr(path, ':');
		if (delimiter == NULL)
			delimiter = ft_strchr(path, '\0');
		directory = string_sub(s, path, delimiter - path);
		filename = string_join(s, directory, command);
		if (access(filename, F_OK | X_OK) == 0)
			return (filename);
		path = delimiter + (*delimiter == ':');
	}
	return (NULL);
}

void	subprocess_run(t_shell *s, t_vec *command)
{
	char *const	name = command->data[0];
	char *const	filename = get_command_filename(s, name);
	char		*error_msg;

	if (filename == NULL || access(filename, F_OK) != 0)
	{
		error_msg = string_join(s, name, ": command not found");
		shell_exit(s, 127, error_msg);
	}
	execve(filename, (char **) command->data, (char **) s->envp->data);
	shell_exit(s, 126, strerror(errno));
}
