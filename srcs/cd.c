#include "../incl/minishell.h"

static int	do_chdir(char *path)
{
	if (path[0] == '-' && path[1] == '\0')
	{
		path = getenv("OLDPWD");
		if (!path)
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

int	mini_cd(char **argv, int fd, t_shell *s)
{
	int			i;
	char		*path;

	(void)fd;
	(void)s;
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
		path = getenv("HOME");
		if (!path)
		{
			ft_fprintf(2, "minishell: cd: HOME not set\n");
			return (1);
		}
	}
	else
		path = argv[1];
	return (do_chdir(path));
}
