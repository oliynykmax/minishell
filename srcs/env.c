#include "../incl/minishell.h"

int	mini_env(char **argv, int fd, t_shell *s, char **envp)
{
	int	i;

	i = 0;
	(void)argv;
	(void)s;
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
