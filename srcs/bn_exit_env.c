#include "../incl/minishell.h"
/*
 * Checks if a string is a valid integer (like bash exit).
 */
static int	str_isdigit(const char *s)
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
 * Exits the shell with the given status code.
 */
int	mini_exit(char **args, int fd, t_shell *s)
{
	int	has_second_arg;

	has_second_arg = (args[2] != NULL);
	ft_putstr_fd("exit\n", fd);
	if (args[1] == NULL)
		shell_exit(s, s->last_status, NULL);
	if (!str_isdigit(args[1]))
	{
		ft_fprintf(fd, "minishell: exit: %s: numeric argument required\n",
			args[1]);
		shell_exit(s, 2, NULL);
	}
	if (has_second_arg)
	{
		ft_fprintf(fd, "minishell: exit: too many arguments\n");
		s->last_status = 1;
		return (1);
	}
	shell_exit(s, (unsigned char)ft_atoi(args[1]), NULL);
	return (0);
}

/*
 * prints out all the envp variables that has some value,
 * not just key
 */
int	mini_env(char **argv, int fd, t_shell *s)
{
	int		i;
	char	**env;

	(void)argv;
	if (s == NULL)
		return (1);
	if (s->envp == NULL)
		return (1);
	env = (char **)s->envp->data;
	if (env == NULL)
		return (0);
	i = 0;
	while (env[i] != NULL)
	{
		if (ft_strchr(env[i], '='))
		{
			ft_putstr_fd(env[i], fd);
			ft_putstr_fd("\n", fd);
		}
		i++;
	}
	return (0);
}
