#include "../incl/minishell.h"
#include <stdlib.h>

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

void	export_sort_and_print(char **envp)
{
	int		i;
	char	**sorted_envp;

	i = 0;
	sorted_envp = NULL;
	sorted_envp = sort_envp(envp);
	while (sorted_envp[i])
	{
		printf("declare -x %s\n", sorted_envp[i]);
		i++;
	}
	free(sorted_envp);
}

int	export(int argc, char **argv, char **envp)
{
	if (!envp)
		return (EXIT_FAILURE);
	(void)argv;
	if (argc == 1)
	{
		export_sort_and_print(envp);
		return (0);
	}
	return (0);
}
