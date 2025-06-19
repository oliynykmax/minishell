#include "../incl/minishell.h"

// Apply a file-based redirection (covers "<", ">", and ">>"). Opens the given
// file with the provided flags, and redirects it to a target file descriptor
// (which should normally be stdout or stdin).

bool	redirect_file(t_shell *s, char *filename, int open_flags, int target)
{
	const int	file = open(filename, open_flags, 0644);

	if (file == -1)
	{
		ft_fprintf(2, "minishell: %s: %s\n", filename, strerror(errno));
		s->last_status = 1;
		return (false);
	}
	dup2(file, target);
	close(file);
	return (true);
}

// Apply all redirections in a vector containing pairs of redirection operators
// followed by filenames. The redirections are applied to the current process'
// std file descriptors.

bool	redirect(t_shell *s, t_vec *redirections)
{
	char	**str;

	if (redirections->size == 0)
		return (true);
	str = (char **)redirections->data;
	while (*str != NULL)
	{
		if (!ft_strcmp(*str, "<<") && !redirect_file(s, *++str, O_RDONLY, 0))
			return (false);
		if (!ft_strcmp(*str, "<") && !redirect_file(s, *++str, O_RDONLY, 0))
			return (false);
		if (!ft_strcmp(*str, ">") && !redirect_file(s, *++str,
				O_WRONLY | O_CREAT | O_TRUNC, 1))
			return (false);
		if (!ft_strcmp(*str, ">>") && !redirect_file(s, *++str,
				O_WRONLY | O_CREAT | O_APPEND, 1))
			return (false);
		str++;
	}
	return (true);
}
