#include "minishell.h"

static int	is_blank(char c)
{
	return (c == ' ' || c == '\t' || c == '\n');
}

static int	is_meta(char c)
{
	return (is_blank(c) || c == '|' || c == '&' || c == '<' || c == '>'
		|| c == '(' || c == ')');
}

static char	*tokenize_meta(t_shell *s, t_vec *tokens, char *i)
{
	int	paired;

	paired = i[0] == i[1] && (*i == '<' || *i == '>' || *i == '&' || *i == '|');
	vector_push(tokens, string_sub(s, i, paired + 1));
	return (i + paired + 1);
}

static char	*tokenize_word(t_shell *s, t_vec *tokens, char *input)
{
	char *const begin = input;
	char		del;

	del = '\0';
	if (*input == '\'' || *input == '\"')
		del = *input++;
	while (*input != '\0' && *input != del && (del != '\0' || !is_meta(*input)))
		input++;
	input += *input == del && del != '\0';
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
