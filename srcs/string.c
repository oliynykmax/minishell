#include "minishell.h"

char	*string_new(t_shell *s, const char *string)
{
	const size_t	length = ft_strlen(string);
	char *const		result = shell_malloc(s, length + 1);
	return (ft_memcpy(result, string, length));
}

char	*string_join(t_shell *s, const char *a, const char *b)
{
	const size_t	a_length = ft_strlen(a);
	const size_t	b_length = ft_strlen(b);
	char *const		result = shell_malloc(s, a_length + b_length + 1);
	ft_memcpy(result + a_length, b, b_length);
	return (ft_memcpy(result, a, a_length));
}
