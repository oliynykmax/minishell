#include "../incl/minishell.h"

static int	export_print(char **envp, int fd)
{
	int		i;
	char	*equals_pos;

	i = 0;
	if (!envp)
		return (0);
	sort_strings(envp);
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

static int	is_invalid_export_identifier(char *var, int *status)
{
	if (var && var[0] == '-')
	{
		ft_fprintf(2, "minishell: export: `%s': not a valid identifier\n", var);
		*status = 2;
		return (1);
	}
	return (0);
}

static int	is_valid_var(char *var, int *status)
{
	int	i;

	if (is_invalid_export_identifier(var, status))
		return (0);
	i = 1;
	if (!var || !var[0] || (!ft_isalpha(var[0]) && var[0] != '_'))
	{
		ft_fprintf(2, "minishell: export: `%s': not a valid identifier", var);
		*status = 1;
		return (0);
	}
	while (var[i] && var[i] != '=')
	{
		if (!ft_isalnum(var[i]) && var[i] != '_')
		{
			ft_fprintf(2, "minishell: export: `%s': not a valid identifier",
				var);
			*status = 1;
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
		return (export_print((char **)s->envp->data, fd));
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
