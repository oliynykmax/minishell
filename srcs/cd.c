#include "../incl/minishell.h"
/*
 * This file contains the implementation of the cd command.
 * returns 0 on success, 1 on failure
 * gets local envp variable from our envp, not the system wide one
 */
static int	do_chdir(char *path, t_shell *s)
{
	if (path[0] == '-' && path[1] == '\0')
	{
		path = get_env_variable(s, "OLDPWD");
		if (!path || !path[0])
		{
			ft_fprintf(2, "minishell: cd: OLDPWD not set\n");
			return (1);
		}
	}
	if (chdir(path) != 0)
	{
		perror("minishell: cd");
		return (1);
	}
	return (0);
}
/*
 * doesn't needs fd, only argv and shell struct are essential
 * accepts 0 or 1 argument, works with "cd -"
 */

int	mini_cd(char **argv, int fd, t_shell *s)
{
	int			i;
	char		*path;

	(void)fd;
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
	return (do_chdir(path, s));
}
