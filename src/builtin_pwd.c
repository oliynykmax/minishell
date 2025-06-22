#include "minishell.h"

int	mini_pwd(char **argv, t_shell *s)
{
	char	*cwd;
	int		i;

	i = 1;
	(void)s;
	while (argv[i])
	{
		if (argv[i][0] == '-')
		{
			ft_fprintf(STDERR_FILENO, "pwd: options are not supported\n");
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
	ft_fprintf(STDOUT_FILENO, "%s\n", cwd);
	free(cwd);
	return (0);
}
