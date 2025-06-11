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

	if (!ep)
		return (NULL);
	len = 0;
	while (ep[len])
		len++;
	if (len == 0)
		return (NULL);
	sorted = malloc((len + 1) * sizeof(char *));
	if (!sorted)
		return (NULL);
	i = -1;
	while (++i < len)
		sorted[i] = ep[i];
	sorted[len] = NULL;
	return (sort_chars(sorted, len));
}

void	export_sort_and_print(char **envp, int fd)
{
	int		i;
	char	**sorted_envp;
	char	*equals_pos;

	if (!envp)
		return ;
	i = 0;
	sorted_envp = sort_envp(envp);
	if (!sorted_envp)
		return ;
	while (sorted_envp[i])
	{
		equals_pos = ft_strchr(sorted_envp[i], '=');
		if (equals_pos)
		{
			*equals_pos = '\0';
			ft_fprintf(fd, "declare -x %s=\"%s\"\n", sorted_envp[i], equals_pos
				+ 1);
			*equals_pos = '=';
		}
		else
			ft_fprintf(fd, "declare -x %s\n", sorted_envp[i]);
		i++;
	}
	free(sorted_envp);
}

void	insert_into_envp(char *var, t_shell *s, int var_len)
{
	const char		**envp = (const char **)s->envp->data;
	char			*equals_pos;
	int				i;

	if (!envp)
		return (vector_push(s->envp, string_new(s, var)));
	i = -1;
	while (envp[++i])
	{
		equals_pos = ft_strchr(envp[i], '=');
		if ((equals_pos && ft_strncmp(envp[i], var, var_len) == 0
				&& equals_pos - envp[i] == var_len)
			|| (!equals_pos && ft_strncmp(envp[i], var, var_len) == 0
				&& (int)ft_strlen(envp[i]) == var_len))
		{
			s->envp->data[i] = string_new(s, var);
			return ;
		}
	}
	vector_push(s->envp, string_new(s, var));
}

int	mini_export(char **argv, int fd, t_shell *s)
{
	char	*equals_pos;
	int		var_len;
	int		i;

	if (!s || !s->envp)
		return (EXIT_FAILURE);
	if (argv[1] == NULL)
		export_sort_and_print((char **)s->envp->data, fd);
	else
	{
		i = 1;
		while (argv[i])
		{
			equals_pos = ft_strchr(argv[i], '=');
			if (equals_pos)
				var_len = equals_pos - argv[i];
			else
				var_len = ft_strlen(argv[i]);
			insert_into_envp(argv[i], s, var_len);
			i++;
		}
	}
	return (0);
}
