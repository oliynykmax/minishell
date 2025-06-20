#include "../incl/minishell.h"

int	validate_redirections(t_vec *tokens, t_shell *s)
{
	size_t	i;
	char	*tok;

	i = 0;
	while (i < tokens->size)
	{
		tok = tokens->data[i];
		if (tok && (!ft_strcmp(tok, "<") || !ft_strcmp(tok, ">")
				|| !ft_strcmp(tok, "<<") || !ft_strcmp(tok, ">>")))
		{
			if (i + 1 >= tokens->size
				|| is_meta(((char **)tokens->data)[i + 1][0]))
			{
				ft_fprintf(2, "minishell: syntax error\n");
				s->last_status = 2;
				return (0);
			}
		}
		i++;
	}
	return (1);
}
