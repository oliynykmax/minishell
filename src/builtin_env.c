#include "minishell.h"

/*
 * prints out all the envp variables that has some value,
 * not just key
 */
int	mini_env(char **argv, t_shell *s)
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
		if (ft_strchr(env[i], '='))
		{
			ft_fprintf(STDOUT_FILENO, "%s\n", env[i]);
		}
		i++;
	}
	return (0);
}
