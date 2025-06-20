#include "../incl/minishell.h"

int	heredoc_delim_is_quoted(char *delim)
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

char	*heredoc_strip_quotes(t_shell *s, char *delim)
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

int	heredoc_is_delimiter(char *line, char *delim)
{
	if (!line)
		return (1);
	if (ft_strcmp(line, delim) == 0)
		return (1);
	return (0);
}

int	heredoc_replace_tokens(t_shell *s, t_vec *tokens)
{
	char			*delim;
	size_t			i;
	char **const	str = (char **)tokens->data;

	i = 0;
	while (i < tokens->size)
	{
		if (ft_strcmp(str[i], "<<") == 0 && str[i + 1] != NULL)
		{
			if (str[i + 1][0] == '<')
				break ;
			delim = heredoc(str[i + 1], s);
			if (!delim)
				return (1);
			tokens->data[i + 1] = delim;
		}
		i++;
	}
	return (0);
}

int	heredoc_check_limit(t_shell *s, t_vec *tokens)
{
	size_t	i;
	int		heredoc_count;

	heredoc_count = 0;
	i = 0;
	clear_temp_files(s);
	while (i < tokens->size)
		if (ft_strcmp(tokens->data[i++], "<<") == 0)
			heredoc_count++;
	if (heredoc_count > 16)
	{
		ft_fprintf(2, "minishell: maximum here-document count exceeded\n");
		shell_exit(s, 2, NULL);
	}
	if (heredoc_replace_tokens(s, tokens))
	{
		s->last_status = 130;
		return (1);
	}
	return (0);
}
