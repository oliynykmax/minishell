#include "../incl/minishell.h"

int	env(char **envp, int fd)
{
	int	i;

	i = 0;
	if (envp == NULL)
		return (1);
	else
	{
		while (envp[i])
		{
			ft_putstr_fd(envp[i], fd);
			ft_putstr_fd("\n", fd);
			i++;
		}
		return (0);
	}
}
