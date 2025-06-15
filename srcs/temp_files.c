#include "../incl/minishell.h"

char	*create_temp_file(t_shell *s)
{
	char		*temp_path;
	char		*temp_name;
	static int	file_counter = 0;

	temp_name = string_join(s, "/tmp/minishell_heredoc_", string_itoa(s,
				file_counter++));
	temp_path = string_new(s, temp_name);
	return (temp_path);
}

int	clear_temp_files(char *filename)
{
	if (!filename)
		return (-1);
	if (access(filename, F_OK) != 0)
		return (-1);
	if (unlink(filename) == -1)
		return (-1);
	return (0);
}
