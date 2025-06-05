#include "../incl/minishell.h"

int	main(void)
{
	char	*rl;

	while (1)
	{
		rl = readline("macroshell> ");
		if (ft_strcmp(rl, "exit") == 0)
			return (0);
	}
	return (0);
}
