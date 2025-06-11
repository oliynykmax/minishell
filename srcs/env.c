#include "../incl/minishell.h"

int	mini_env(char **argv, int fd, t_shell *s)
{
	int		i;
	char	**env;

	(void)argv;
	if (s == NULL)
		return (1);
	if (s->envp == NULL)
		return (1);
	env = (char **)s->envp->data;
	if (env == NULL)
		return (0);
	i = 0;
	while (env[i] != NULL)
	{
		ft_putstr_fd(env[i], fd);
		ft_putstr_fd("\n", fd);
		i++;
	}
	return (0);
}
