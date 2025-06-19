#include "../incl/minishell.h"

static int	split_word(t_vec *t, int i)
{
	char	*s;
	char	*start;
	char	quote;

	s = t->data[i];
	vector_delete(t, i);
	while (*s != '\0')
	{
		while (is_blank(*s))
			s++;
		start = s;
		quote = 0;
		while (*s != '\0' && (quote || !is_blank(*s)))
		{
			if (!quote && (*s == '\'' || *s == '\"'))
				quote = *s;
			else if (*s == quote)
				quote = 0;
			s++;
		}
		if (start < s)
			vector_insert(t, i++, string_sub(t->shell, start, s - start));
	}
	return (i);
}

void	split_words(t_vec *t)
{
	size_t	i;

	i = 0;
	while (i < t->size)
		i = split_word(t, i);
}
