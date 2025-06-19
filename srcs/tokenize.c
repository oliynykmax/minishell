#include "../incl/minishell.h"

static char	*tokenize_meta(t_shell *s, t_vec *tokens, char *i)
{
	const int	paired = i[0] == i[1] && (i[0] == '<' || i[0] == '>');

	vector_push(tokens, string_sub(s, i, paired + 1));
	return (i + paired + 1);
}

static char	*tokenize_word(t_shell *s, t_vec *tokens, char *input)
{
	char *const	begin = input;
	char		quote;

	quote = 0;
	while (*input != '\0')
	{
		if (!quote)
		{
			if (*input == '\'' || *input == '\"')
				quote = *input;
			if (is_meta(*input) || is_blank(*input))
				break ;
		}
		else if (*input == quote)
			quote = 0;
		input++;
	}
	vector_push(tokens, string_sub(s, begin, input - begin));
	return (input);
}

static bool	is_syntax_error(t_vec *tokens)
{
	char **const	str = (char**) tokens->data;
	char			*last;
	size_t			i;

	i = 0;
	while (++i < tokens->size)
		if (!strcmp(str[i - 1], "|") && !strcmp(str[i], "|"))
			return (true);
	if (tokens->size == 0)
		return (false);
	last = str[tokens->size - 1];
	if (*last == '<' || *last == '>')
		return (true);
	return (str[0][0] == '|');
}

int	replace_heredoc(t_shell *s, t_vec *tokens)
{
	char **const	str = (char**)tokens->data;
	char			*delim;
	size_t			i;

	i = 0;
	while (i < tokens->size)
	{
		if (ft_strcmp(str[i], "<<") == 0)
		{
			delim = heredoc(str[i+1], s);
			if (!delim)
				return (1);
			tokens->data[i+1] = delim;
		}
		i++;
	}
	return (0);
}

static int	check_heredoc_limit(t_shell *s, t_vec *tokens)
{
	size_t	i;
	int		heredoc_count;

	heredoc_count = 0;
	i = 0;
	while (i < tokens->size)
		if (ft_strcmp(tokens->data[i++], "<<") == 0)
			heredoc_count++;
	if (heredoc_count > 16)
	{
		ft_printf("minishell: maximum here-document count exceeded\n");
		shell_exit(s, 2, NULL);
	}
	if (replace_heredoc(s, tokens))
	{
		s->last_status = 130 + (heredoc_count > 1) * 9;
		return (1);
	}
	return (0);
}

t_vec	*tokenize(t_shell *s, char *input)
{
	t_vec *const	tokens = vector_new(s, 0);

	while (*input != '\0')
	{
		while (is_blank(*input))
			input++;
		if (is_meta(*input))
			input = tokenize_meta(s, tokens, input);
		else if (*input != '\0')
			input = tokenize_word(s, tokens, input);
	}
	/// needs to be in other place, also handle the return value
	if (check_heredoc_limit(s, tokens))
		return (NULL);
	///yeah
	if (is_syntax_error(tokens))
	{
		printf("minishell: syntax error\n");
		return (NULL);
	}
	return (tokens);
}
