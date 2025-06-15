#include "../incl/minishell.h"

static char	*sigint_heredoc(char *line, int fd, char *temp_path, t_shell *s)
{
	close(fd);
	clear_temp_files(temp_path);
	if (line)
		free(line);
	g_signal = 0;
	s->last_status = 130;
	return (NULL);
}

static void	write_to_temp_file(int fd, char *line)
{
	if (ft_strlen(line) == 0)
		write(fd, "\n", 1);
	else if (ft_strlen(line) > 0)
	{
		write(fd, line, ft_strlen(line));
		write(fd, "\n", 1);
	}
	if (line)
		free(line);
}

static int	setup_heredoc_fd_and_signals(int *fd, const char *temp_path)
{
	*fd = open(temp_path, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (*fd == -1)
	{
		perror("open");
		return (-1);
	}
	setup_heredoc_signals_local();
	return (0);
}

static char	*process_heredoc_line(char *line, char *delim, int delim_len,
		int fd)
{
	if (!line)
	{
		ft_fprintf(2,
			"minishell: warning: here-document at"
			"line 1 delimited by end-of-file (wanted `%s')\n",
			delim);
		return (NULL);
	}
	if (ft_strlen(line) == (size_t)delim_len && ft_strcmp(line, delim) == 0)
	{
		free(line);
		return (NULL);
	}
	write_to_temp_file(fd, line);
	return (line);
}

char	*heredoc(char *delim, t_shell *s)
{
	char		*temp_path;
	int			fd;
	char		*line;
	const int	delim_len = ft_strlen(delim);

	temp_path = create_temp_file(s);
	if (setup_heredoc_fd_and_signals(&fd, temp_path) == -1)
		return (NULL);
	while (1)
	{
		line = readline("> ");
		if (g_signal == SIGINT)
			return (sigint_heredoc(line, fd, temp_path, s));
		if (!process_heredoc_line(line, delim, delim_len, fd))
			break ;
	}
	close(fd);
	return (temp_path);
}
