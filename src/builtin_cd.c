#include "minishell.h"

/*
 * doesn't need fd, only argv and shell struct are essential
 * accepts 0 or 1 argument, works with "cd -"
 */

int	mini_cd(char **argv, t_shell *s)
{
	int		i;
	char	*path;

	i = 0;
	while (argv[i])
		i++;
	if (i > 2)
	{
		ft_fprintf(2, "minishell: cd: too many arguments\n");
		return (1);
	}
	if (!argv[1])
	{
		path = get_env_variable(s, "HOME");
		if (!path || !path[0])
		{
			ft_fprintf(2, "minishell: cd: HOME not set\n");
			return (1);
		}
	}
	else
		path = argv[1];
	return (change_directory(path, s));
}