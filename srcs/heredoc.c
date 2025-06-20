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
	int	i;
	int	counter_single;
	int	counter_double;

	i = 0;
	counter_single = 0;
	counter_double = 0;
	while (delim[i])
	{
		if (delim[i] == '"')
			counter_double++;
		if (delim[i] == '\'')
			counter_single++;
		i++;
	}
	if (counter_double > 1 || counter_single > 1)
		return (1);
	return (0);
}

static char	*get_clean_delim(t_shell *s, char *delim)
{
	char	*new_delim;
	int		i;
	int		j;

	new_delim = shell_malloc(s, ft_strlen(delim) + 1);
	i = 0;
	j = 0;
	while (delim[j])
	{
		if (delim[j] != '"' && delim[j] != '\'')
		{
			new_delim[i] = delim[j];
			i++;
		}
		j++;
	}
	new_delim[i] = '\0';
	return (new_delim);
}

static int	is_delimiter(char *line, char *delim)
{
	if (!line)
		return (1);
	if (ft_strcmp(line, delim) == 0)
		return (1);
	return (0);
}

static int	read_heredoc_handle(t_heredoc *h, char *line)
{
	if (g_signal == SIGINT)
		return (0);
	if (is_delimiter(line, h->clean_delim))
	{
		if (!line)
			ft_fprintf(2, "minishell: warning: heredoc"
				" stopped by EOF (wanted `%s')\n", h->clean_delim);
		return (0);
	}
	write_to_temp_file(h->fd, string_new(h->s, line), h->s, h->expand);
	return (1);
}

static void	read_heredoc_loop(t_heredoc *h)
{
	char	*line;

	while (1)
	{
		if (isatty(STDIN_FILENO))
			line = readline("> ");
		else
			line = shell_readline(h->s, INPUT_NONINTERACTIVE);
		if (!read_heredoc_handle(h, line))
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

	h.clean_delim = get_clean_delim(s, delim);
	h.expand = !is_delim_quoted(delim);
	h.s = s;
	if (setup_heredoc_fd(&h.fd, temp_path) == -1)
		return (NULL);
	read_heredoc_loop(&h);
	close(h.fd);
	if (g_signal == SIGINT)
		return (NULL);
	return ((char *)temp_path);
}
