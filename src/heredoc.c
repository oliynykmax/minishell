#include "minishell.h"

static void	heredoc_write_line(int fd, char *line, t_shell *s, int expand)
{
	char	*content;

	content = string_new(s, line);
	if (expand && ft_strchr(content, '$'))
		content = params_expand_string(s, content);
	write(fd, content, ft_strlen(content));
	write(fd, "\n", 1);
}

static int	heredoc_open_tempfile(int *fd, const char *temp_path)
{
	*fd = open(temp_path, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (*fd == -1)
	{
		perror("open");
		return (-1);
	}
	return (0);
}

static int	heredoc_handle_line(t_heredoc *h, char *line)
{
	if (g_signal == SIGINT)
		return (0);
	if (heredoc_is_delimiter(line, h->clean_delim))
	{
		if (!line)
			ft_fprintf(2, "minishell: warning: heredoc"
				" stopped by EOF (wanted `%s')\n", h->clean_delim);
		return (0);
	}
	heredoc_write_line(h->fd, string_new(h->s, line), h->s, h->expand);
	return (1);
}

static void	heredoc_read_loop(t_heredoc *h)
{
	char	*line;

	while (1)
	{
		if (isatty(STDIN_FILENO))
			line = readline("> ");
		else
			line = shell_readline(h->s, INPUT_NONINTERACTIVE);
		if (!heredoc_handle_line(h, line))
		{
			if (isatty(STDIN_FILENO))
				free(line);
			break ;
		}
		if (isatty(STDIN_FILENO))
			free(line);
	}
}

char	*heredoc(char *delim, t_shell *s)
{
	t_heredoc	h;
	const char	*temp_path = create_temp_file(s);

	h.clean_delim = heredoc_strip_quotes(s, delim);
	h.expand = !heredoc_delim_is_quoted(delim);
	h.s = s;
	if (heredoc_open_tempfile(&h.fd, temp_path) == -1)
		return (NULL);
	heredoc_read_loop(&h);
	close(h.fd);
	if (g_signal == SIGINT)
		return (NULL);
	return ((char *)temp_path);
}
