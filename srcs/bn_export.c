#include "../incl/minishell.h"

static int	export_write_error(const char *var, int *status, int code)
{
	ft_fprintf(2, "minishell: export: `%s': not a valid identifier\n", var);
	if (status)
		*status = code;
	return (0);
}

static int	export_print(char **envp)
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
			ft_fprintf(STDOUT_FILENO, "declare -x %s=\"%s\"\n", envp[i],
				equals_pos + 1);
			*equals_pos = '=';
		}
		else
			ft_fprintf(STDOUT_FILENO, "declare -x %s\n", envp[i]);
		i++;
	}
	return (0);
}

static int	export_print_sorted_copy(t_shell *s)
{
	t_vec	*old_envp;
	t_vec	*copy_vec;
	size_t	i;

	old_envp = s->envp;
	copy_vec = vector_new(s, old_envp->size);
	i = 0;
	while (i < old_envp->size)
	{
		vector_push(copy_vec, string_new(s, old_envp->data[i]));
		i++;
	}
	vector_push(copy_vec, NULL);
	return (export_print((char **)copy_vec->data));
}

static int	is_valid_var(char *var, int *status)
{
	int	i;

	if (var && var[0] == '-')
		return (export_write_error(var, status, 2));
	i = 1;
	if (!var || !var[0] || (!ft_isalpha(var[0]) && var[0] != '_'))
		return (export_write_error(var, status, 1));
	while (var[i] && var[i] != '=')
	{
		if (!ft_isalnum(var[i]) && var[i] != '_')
			return (export_write_error(var, status, 1));
		i++;
	}
	if (*status != 1)
		*status = 0;
	return (1);
}

int	mini_export(char **argv, t_shell *s)
{
	char	*equals_pos;
	int		var_len;
	int		i;
	int		status;

	status = 0;
	if (argv[1] == NULL)
		return (export_print_sorted_copy(s));
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
