#include "../incl/minishell.h"

static void	write_to_temp_file(int fd, char *line, t_shell *s, int expand)
{
	char	*content;

	content = string_new(s, line);
	if (expand && ft_strchr(content, '$'))
		content = params_expand_string(s, content);
	write(fd, content, ft_strlen(content));
	write(fd, "\n", 1);
}

static int	setup_heredoc_fd(int *fd, const char *temp_path)
{
	*fd = open(temp_path, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (*fd == -1)
	{
		perror("open");
		return (-1);
	}
	return (0);
}

static int	is_delim_quoted(char *delim)
{
	int	len;

	len = ft_strlen(delim);
	if (len >= 2 && delim[0] == '\'' && delim[len - 1] == '\'')
		return (1);
	if (len >= 2 && delim[0] == '"' && delim[len - 1] == '"')
		return (1);
	return (0);
}

static char	*get_clean_delim(t_shell *s, char *delim)
{
	int	len;

	len = ft_strlen(delim);
	if (is_delim_quoted(delim))
		return (string_sub(s, delim + 1, len - 2));
	return (string_new(s, delim));
}

static int	is_delimiter(char *line, char *delim)
{
	if (!line)
		return (1);
	if (ft_strcmp(line, delim) == 0)
		return (1);
	return (0);
}

static void	read_heredoc_loop(int fd, char *clean_delim, t_shell *s, int expand)
{
	char	*line;

	while (1)
	{
		line = readline("> ");
		if (g_signal == SIGINT)
		{
			if (line)
				free(line);
			return ;
		}
		if (is_delimiter(line, clean_delim))
		{
			if (!line)
				ft_fprintf(2, "minishell: warning: heredoc"
					" stopped by EOF (wanted `%s')\n", clean_delim);
			else
				free(line);
			return ;
		}
		write_to_temp_file(fd, line, s, expand);
		free(line);
	}
}

char	*heredoc(char *delim, t_shell *s)
{
	const char	*temp_path = create_temp_file(s);
	const char	*clean_delim = get_clean_delim(s, delim);
	int			fd;
	const int	expand = !is_delim_quoted(delim);

	if (setup_heredoc_fd(&fd, temp_path) == -1)
		return (NULL);
	read_heredoc_loop(fd, (char *)clean_delim, s, expand);
	close(fd);
	if (g_signal == SIGINT)
	{
		s->last_status = 130;
		return (NULL);
	}
	return ((char *)temp_path);
}
