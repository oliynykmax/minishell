#include "../incl/minishell.h"

// Apply a file-based redirection (covers "<", ">", and ">>"). Opens the given
// file with the provided flags, and redirects it to a target file descriptor
// (which should normally be stdout or stdin).

static void	redirect_file(char *filename, int open_flags, int target)
{
	const int	file = open(filename, open_flags, 0644);

	if (file == -1)
	{
		ft_fprintf(2, "minishell: %s: %s", filename, strerror(errno));
		return ;
	}
	dup2(file, target);
	close(file);
}

// Apply all redirections in a vector containing pairs of redirection operators
// followed by filenames. The redirections are applied to the current process'
// std file descriptors.

void	redirect(t_vec *redirections)
{
	char	**str;

	if (redirections->size == 0)
		return ;
	str = (char **)redirections->data;
	while (*str != NULL)
	{
		if (ft_strcmp(*str, "<<") == 0)
			redirect_file(*++str, O_RDONLY, STDIN_FILENO);
		if (ft_strcmp(*str, "<") == 0)
			redirect_file(*++str, O_RDONLY, STDIN_FILENO);
		if (ft_strcmp(*str, ">") == 0)
			redirect_file(*++str, O_WRONLY | O_CREAT | O_TRUNC, STDOUT_FILENO);
		if (ft_strcmp(*str, ">>") == 0)
			redirect_file(*++str, O_WRONLY | O_CREAT | O_APPEND, STDOUT_FILENO);
		str++;
	}
}
