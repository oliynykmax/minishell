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

static int	is_n_flag(const char *arg)
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
	while (argv[i] && is_n_flag(argv[i]))
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
 * This file contains the implementation of the cd command.
 * returns 0 on success, 1 on failure
 * gets local envp variable from our envp, not the system wide one
 */
static int	do_chdir(char *path, t_shell *s)
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
	return (do_chdir(path, s));
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
