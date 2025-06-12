#include "../incl/minishell.h"

void	sort_envp(char **sorted)
{
	int		i;
	int		j;
	int		len;
	char	*tmp;

	len = 0;
	while (sorted[len])
		len++;
	i = -1;
	while (++i < len - 1)
	{
		j = i;
		while (++j < len)
		{
			if (ft_strcmp(sorted[i], sorted[j]) > 0)
			{
				tmp = sorted[i];
				sorted[i] = sorted[j];
				sorted[j] = tmp;
			}
		}
	}
}

int	export_sort_and_print(char **envp, int fd)
{
	int		i;
	char	*equals_pos;

	i = 0;
	if (!envp)
		return (0);
	sort_envp(envp);
	while (envp[i])
	{
		equals_pos = ft_strchr(envp[i], '=');
		if (equals_pos)
		{
			*equals_pos = '\0';
			ft_fprintf(fd, "declare -x %s=\"%s\"\n", envp[i], equals_pos + 1);
			*equals_pos = '=';
		}
		else
			ft_fprintf(fd, "declare -x %s\n", envp[i]);
		i++;
	}
	return (0);
}

void	insert_into_envp(char *var, t_shell *s, int var_len)
{
	const char	**envp = (const char **)s->envp->data;
	char		*equals_pos;
	int			i;

	if (!envp)
		return (vector_push(s->envp, string_new(s, var)));
	i = -1;
	while (envp[++i])
	{
		equals_pos = ft_strchr(envp[i], '=');
		if ((equals_pos && ft_strncmp(envp[i], var, var_len) == 0 && equals_pos
				- envp[i] == var_len) || (!equals_pos && ft_strncmp(envp[i],
					var, var_len) == 0 && (int)ft_strlen(envp[i]) == var_len))
		{
			s->envp->data[i] = string_new(s, var);
			return ;
		}
	}
	vector_push(s->envp, string_new(s, var));
}

int	is_valid_var(char *var, int *status)
{
	int	i;

	i = 1;
	if (!var || !var[0] || (!ft_isalpha(var[0]) && !(var[0] == '_')))
	{
		*status = 1;
		ft_fprintf(2, "minishell: export: `%s': not a valid identifier\n", var);
		return (0);
	}
	while (var[i] && var[i] != '=')
	{
		if (!ft_isalnum(var[i]) && var[i] != '_')
		{
			*status = 1;
			ft_fprintf(2, "minishell: export: `%s': not a valid identifier\n",
				var);
			return (0);
		}
		i++;
	}
	if (*status != 1)
		*status = 0;
	return (1);
}

int	mini_export(char **argv, int fd, t_shell *s)
{
	char	*equals_pos;
	int		var_len;
	int		i;
	int		status;

	status = 0;
	if (argv[1] == NULL)
		return (export_sort_and_print((char **)s->envp->data, fd));
	i = 1;
	while (argv[i])
	{
		if (!is_valid_var(argv[i], &status))
		{
			i++;
			continue ;
		}
		equals_pos = ft_strchr(argv[i], '=');
		if (equals_pos)
			var_len = equals_pos - argv[i];
		else
			var_len = ft_strlen(argv[i]);
		insert_into_envp(argv[i], s, var_len);
		i++;
	}
	return (status);
}
