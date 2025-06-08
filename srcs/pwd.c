#include "../incl/minishell.h"

int	pwd(char **argv)
{
	char	*cwd;
	int		i;

	i = 1;
	while (argv[i])
	{
		if (argv[i][0] == '-')
		{
			ft_putstr_fd("pwd: options are not supported\n", STDERR_FILENO);
			return (2);
		}
		i++;
	}
	cwd = getcwd(NULL, 0);
	if (!cwd)
	{
		perror("pwd");
		return (1);
	}
	ft_printf("%s\n", cwd);
	free(cwd);
	return (0);
}
