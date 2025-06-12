#include "../incl/minishell.h"

// Given the name of a command (like "ls"), get the program filename for that
// command (like "/usr/bin/ls"). This is done by searching the PATH environment
// variable and looking for an executable file matching the command's name in
// each listed directory. Returns NULL if the command can not be found.

static char	*get_command_filename(t_shell *s, char *command)
{
	char	*path;
	char	*delimiter;
	char	*directory;
	char	*filename;

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

void	subprocess_run(t_shell *s, t_vec *command, t_vec *redirs)
{
	char *const	name = command->data[0];
	char *const	filename = get_command_filename(s, name);
	size_t		i;

	(void) redirs;
	if (filename == NULL)
	{
		ft_fprintf(2, "minishell: %s: command not found\n", name);
		return ;
	}
	printf("TODO: Run program %s\n", filename);
	i = 0;
	while (++i < command->size)
		printf("    Argument #%zu: %s\n", i, (char *) command->data[i]);
}
