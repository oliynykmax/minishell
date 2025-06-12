#include "../incl/minishell.h"

int	mini_unset(char **argv, int fd, t_shell *s)
{
	int		i;
	int		j;
	char	**envp;

	(void)fd;
	i = 0;
	if (!argv[1])
		return (0);
	envp = (char **)s->envp->data;
	if (!envp)
		return (0);
	while (argv[i++])
	{
		envp = (char **)s->envp->data;
		j = 0;
		while (envp[j] && ft_strncmp(envp[j], argv[i], ft_strlen(argv[i])) != 0)
			j++;
		if (!envp[j])
			continue ;
		if (envp[j][ft_strlen(argv[i])] == '='
			|| envp[j][ft_strlen(argv[i])] == '\0')
			vector_delete(s->envp, j);
	}
	return (0);
}
