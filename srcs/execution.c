#include "../incl/minishell.h"

t_bn	*get_builtin_by_name(char *name)
{
	if (ft_strcmp(name, "echo") == 0)
		return (mini_echo);
	if (ft_strcmp(name, "cd") == 0)
		return (mini_cd);
	if (ft_strcmp(name, "pwd") == 0)
		return (mini_pwd);
	if (ft_strcmp(name, "export") == 0)
		return (mini_export);
	if (ft_strcmp(name, "unset") == 0)
		return (mini_unset);
	if (ft_strcmp(name, "env") == 0)
		return (mini_env);
	if (ft_strcmp(name, "exit") == 0)
		return (mini_exit);
	return (NULL);
}

void	error(const char *message)
{
	ft_fprintf(2, "minishell: %s\n", message);
}

void	safe_close(int *fd)
{
	if (*fd != -1 && *fd != STDIN_FILENO && *fd != STDOUT_FILENO)
	{
		close(*fd);
		*fd = -1;
	}
}
