#include "../incl/minishell.h"
/*
 * set invalid is used by
 * export command
 */
int	set_invalid(char *var, int *status)
{
	ft_fprintf(2, "minishell: export: `%s': not a valid identifier\n", var);
	*status = 1;
	return (0);
}

/*
 * prints out all the envp variables that has some value,
 * not just key
 */
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
		if (ft_strchr(env[i], '='))
		{
			ft_putstr_fd(env[i], fd);
			ft_putstr_fd("\n", fd);
		}
		i++;
	}
	return (0);
}
