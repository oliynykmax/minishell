#include "../incl/minishell.h"

char	*create_temp_file(t_shell *s)
{
	char		*temp_name;
	int	file_counter = 0;

	temp_name = string_join(s, "/tmp/m_h_", string_itoa(s, file_counter));
	while (access(temp_name, F_OK) == 0)
		temp_name = string_join(s, "/tmp/m_h_", string_itoa(s, file_counter++));
	file_counter = 0;
	return (temp_name);
}

void	clear_temp_files(t_shell *s)
{
	char	*temp_name;
	int		file_counter;

	file_counter = 0;
	temp_name = string_join(s, "/tmp/m_h_", string_itoa(s, file_counter));
	while (access(temp_name, F_OK) == 0)
	{
		if (access(temp_name, F_OK) == 0)
			unlink(temp_name);
		file_counter++;
		temp_name = string_join(s, "/tmp/m_h_", string_itoa(s, file_counter));
	}
}
