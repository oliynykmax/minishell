#include "../incl/minishell.h"

int	env(char **envp)
{
	int	i;

	i = 0;
	if (envp == NULL)
		return (1);
	else
	{
		while (envp[i])
		{
			ft_printf("%s\n", envp[i]);
			i++;
		}
		return (0);
	}
}
