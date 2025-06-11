#include "../incl/minishell.h"

int	mini_echo(char **argv, int fd, t_shell *s, char **envp)
{
	int			i;
	const bool	if_new_line = ft_strcmp(argv[1], "-n") == 0;

	i = 1 + if_new_line;
	while (argv[i])
	{
		ft_fprintf(fd, "%s", argv[i]);
		if (argv[i + 1] != NULL)
			ft_fprintf(fd, " ");
		i++;
	}
	if (!if_new_line)
		ft_fprintf(fd, "\n");
	(void)s;
	(void)envp;
	return (0);
}
