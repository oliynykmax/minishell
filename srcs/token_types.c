#include "../incl/tokenize.h"

int	is_blank(char c)
{
	return (c == ' ' || c == '\t' || c == '\n');
}

int	is_meta(char c)
{
	return (is_blank(c) || c == '|' || c == '&' || c == '<' || c == '>'
		|| c == '(' || c == ')');
}
