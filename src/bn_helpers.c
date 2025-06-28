#include "minishell.h"

/*
 * Sorts an array of strings lexicographically.
 */
void	sort_strings(char **sorted)
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

/*
 * Inserts or updates an environment variable in the shell's envp.
 */
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
			if (ft_strchr(var, '=') != NULL)
				s->envp->data[i] = string_new(s, var);
			return ;
		}
	}
	vector_push(s->envp, string_new(s, var));
}

/*
 * Checks if the argument is a valid -n flag for echo.
 */
int	echo_is_n_flag(const char *arg)
{
	int	i;

	if (!arg)
		return (0);
	if (arg[0] != '-' || arg[1] != 'n')
		return (0);
	i = 1;
	while (arg[++i])
		if (arg[i] != 'n')
			return (0);
	return (1);
}

/*
 * Checks if a string is a valid integer (like bash exit).
 */
int	is_numeric_string(const char *s)
{
	long	result;
	int		i;

	if (!s || !*s)
		return (0);
	i = 0;
	while (s[i] && is_blank(s[i]))
		i++;
	if (s[i] == '-' || s[i] == '+')
		i++;
	if (!s[i] || !ft_isdigit(s[i]))
		return (0);
	while (s[i] && ft_isdigit(s[i]))
		i++;
	while (s[i] && is_blank(s[i]))
		i++;
	if (s[i] != '\0')
		return (0);
	return (ft_atoi_safe(s, &result));
}

/*
 * Changes the current directory, handling OLDPWD and PWD updates.
 */
int	change_directory(char *path, t_shell *s)
{
	int		if_dash;
	char	*pwd;

	if_dash = path[0] == '-' && path[1] == '\0';
	if (if_dash)
	{
		path = get_env_variable(s, "OLDPWD");
		if (!path || !path[0])
		{
			ft_fprintf(2, "minishell: cd: OLDPWD not set\n");
			return (1);
		}
	}
	pwd = get_working_dir(s);
	if (chdir(path) != 0)
	{
		ft_fprintf(2, "minishell: cd: %s: ", path);
		perror(NULL);
		return (1);
	}
	insert_into_envp(string_join(s, "PWD=", get_working_dir(s)), s, 3);
	insert_into_envp(string_join(s, "OLDPWD=", pwd), s, 6);
	if (if_dash)
		ft_fprintf(1, "%s\n", path);
	return (0);
}
