#include "../incl/minishell.h"

int	is_blank(char c)
{
	return (c == ' ' || c == '\t' || c == '\n');
}

int	is_meta(char c)
{
	return (c == '|' || c == '<' || c == '>');
}
