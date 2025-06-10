#include "../incl/minishell.h"

static void	print_tokens(t_vec *tokens)
{
	size_t	i;

	i = 0;
	while (i < tokens->size)
	{
		printf("[%s] ", (char *)tokens->data[i]);
		i++;
	}
	printf("\n");
}

void	shell_execute(t_shell *s)
{
	if (s->tokens->size == 0)
		return ;
	print_tokens(s->tokens);
	if (ft_strcmp(s->tokens->data[0], "exit") == 0)
		shell_exit(s, EXIT_SUCCESS, NULL);
}
