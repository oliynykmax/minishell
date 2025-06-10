#include "../incl/minishell.h"

char	**sort_chars(char **sorted, int len)
{
	int		i;
	int		j;
	char	*tmp;

	i = -1;
	while (++i < len - 1)
	{
		j = -1;
		while (++j < len - i - 1)
		{
			if (ft_strcmp(sorted[j], sorted[j + 1]) > 0)
			{
				tmp = sorted[j];
				sorted[j] = sorted[j + 1];
				sorted[j + 1] = tmp;
			}
		}
	}
	return (sorted);
}

char	**sort_envp(char **ep)
{
	char	**sorted;
	int		len;
	int		i;

	len = 0;
	while (ep[len])
		len++;
	sorted = malloc((len + 1) * sizeof(char *));
	if (!sorted)
		return (NULL);
	i = -1;
	while (++i < len + 1)
		sorted[i] = ep[i];
	return (sort_chars(sorted, len));
}

void	export_sort_and_print(char **envp, int fd)
{
	int		i;
	char	**sorted_envp;
	char	*equals_pos;

	i = 0;
	sorted_envp = NULL;
	sorted_envp = sort_envp(envp);
	while (sorted_envp[i])
	{
		equals_pos = ft_strchr(sorted_envp[i], '=');
		if (equals_pos)
		{
			*equals_pos = '\0';
			ft_fprintf(fd, "declare -x %s=\"%s\"\n", sorted_envp[i],
				equals_pos + 1);
			*equals_pos = '=';
		}
		else
			ft_fprintf(fd, "declare -x %s\n", sorted_envp[i]);
		i++;
	}
	free(sorted_envp);
}

int	mini_export(int fd, char **argv, char **envp)
{
	if (!envp)
		return (EXIT_FAILURE);
	(void)argv;
	export_sort_and_print(envp, fd);
	return (0);
}
