#include "minishell.h"

int	mini_unset(char **argv, t_shell *s)
{
	char	**envp;
	int		j;

	if (!argv[1] || !s->envp->data)
		return (0);
	while (*(++argv))
	{
		envp = (char **)s->envp->data;
		j = 0;
		while (envp[j] && (ft_strncmp(envp[j], *argv, ft_strlen(*argv))
				|| (envp[j][ft_strlen(*argv)] != '='
				&& envp[j][ft_strlen(*argv)] != '\0')))
			j++;
		if (envp[j])
			vector_delete(s->envp, j);
	}
	return (0);
}
