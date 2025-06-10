#include "../incl/minishell.h"

static int	is_blank(char c)
{
	return (c == ' ' || c == '\t' || c == '\n');
}

static int	is_meta(char c)
{
	return (c == '|' || c == '<' || c == '>');
}

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
	return (tokens);
}
