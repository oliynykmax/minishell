#include "../incl/minishell.h"

static char	*tokenize_meta(t_shell *s, t_vec *tokens, char *i)
{
	const int	paired = i[0] == i[1] && (i[0] == '<' || i[0] == '>');

	vector_push(tokens, string_sub(s, i, paired + 1));
	return (i + paired + 1);
}

static char	*tokenize_word(t_shell *s, t_vec *tokens, char *input)
{
	char			quote;
	char *const		begin = input;

	quote = 0;
	while (*input != '\0')
	{
		if (*input == '\\')
			input++;
		else if (!quote)
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
	char			*last;
	size_t			i;
	char **const	str = (char **)tokens->data;

	i = 0;
	while (++i < tokens->size)
		if (!ft_strcmp(str[i - 1], "|") && !ft_strcmp(str[i], "|"))
			return (true);
	if (tokens->size == 0)
		return (false);
	last = str[tokens->size - 1];
	if (*last == '<' || *last == '>')
		return (true);
	return (str[0][0] == '|');
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
	if (heredoc_check_limit(s, tokens))
		return (NULL);
	if (is_syntax_error(tokens))
	{
		ft_fprintf(2, "minishell: syntax error\n");
		s->last_status = 2;
		return (NULL);
	}
	if (!validate_redirections(tokens, s))
		return (NULL);
	return (tokens);
}
