#include "minishell.h"

void	exit_with_msg(t_shell *s, const char *name, const char *msg, int code)
{
	char	*full;

	full = string_join(s, name, msg);
	shell_exit(s, code, full);
}

void	handle_not_found(t_shell *s, const char *name)
{
	if (ft_strchr(name, '/'))
		exit_with_msg(s, name, ": No such file or directory", 127);
	else
		exit_with_msg(s, name, ": command not found", 127);
}

void	handle_stat_cases(t_shell *s, const char *filename, struct stat *st)
{
	if (S_ISDIR(st->st_mode))
		exit_with_msg(s, filename, ": Is a directory", 126);
	if (access(filename, X_OK) != 0)
		exit_with_msg(s, filename, ": Permission denied", 126);
}
