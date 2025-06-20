#include "../incl/minishell.h"

int	mini_unset(char **argv, int fd, t_shell *s)
{
	char	**envp;
	int		j;

	(void)fd;
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

/*
 * echo command
 * always returns 0
 * writes to the file descriptor passed as argument
 */
int	mini_echo(char **argv, int fd, t_shell *s)
{
	int	i;
	int	if_new_line;

	if_new_line = 1;
	i = 1;
	while (argv[i] && echo_is_n_flag(argv[i]))
	{
		if_new_line = 0;
		i++;
	}
	while (argv[i])
	{
		ft_fprintf(fd, "%s", argv[i]);
		if (argv[i + 1])
			ft_fprintf(fd, " ");
		i++;
	}
	if (if_new_line)
		ft_fprintf(fd, "\n");
	(void)s;
	return (0);
}

/*
 * doesn't needs fd, only argv and shell struct are essential
 * accepts 0 or 1 argument, works with "cd -"
 */

int	mini_cd(char **argv, int fd, t_shell *s)
{
	int		i;
	char	*path;

	(void)fd;
	i = 0;
	while (argv[i])
		i++;
	if (i > 2)
	{
		ft_fprintf(2, "minishell: cd: too many arguments\n");
		return (1);
	}
	if (!argv[1])
	{
		path = get_env_variable(s, "HOME");
		if (!path || !path[0])
		{
			ft_fprintf(2, "minishell: cd: HOME not set\n");
			return (1);
		}
	}
	else
		path = argv[1];
	return (change_directory(path, s));
}

int	mini_pwd(char **argv, int fd, t_shell *s)
{
	char	*cwd;
	int		i;

	i = 1;
	(void)s;
	while (argv[i])
	{
		if (argv[i][0] == '-')
		{
			ft_putstr_fd("pwd: options are not supported\n", STDERR_FILENO);
			return (2);
		}
		i++;
	}
	cwd = getcwd(NULL, 0);
	if (!cwd)
	{
		perror("pwd");
		return (1);
	}
	ft_fprintf(fd, "%s\n", cwd);
	free(cwd);
	return (0);
}
