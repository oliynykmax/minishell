#include "../incl/minishell.h"

char	*string_new(t_shell *s, const char *string)
{
	return (string_sub(s, string, ft_strlen(string)));
}

char	*string_sub(t_shell *s, const char *string, size_t length)
{
	char *const	result = shell_malloc(s, length + 1);

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

char	*string_itoa(t_shell *s, int value)
{
	char	*str;
	int		digits;
	long	temp;

	temp = value;
	if (value < 0)
		temp = -temp;
	digits = value <= 0;
	while (temp > 0)
	{
		digits++;
		temp /= 10;
	}
	str = shell_malloc(s, digits + 1);
	temp = value;
	if (value < 0)
		temp = -temp;
	while (digits-- > 0)
	{
		str[digits] = '0' + temp % 10;
		temp /= 10;
	}
	if (value < 0)
		str[0] = '-';
	return (str);
}

int	is_not_empty(const char *str)
{
	while (*str)
	{
		if (is_blank(*str))
			return (0);
		str++;
	}
	return (1);
}
