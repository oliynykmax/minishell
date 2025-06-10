#include "../incl/minishell.h"

static int	str_isdigit(const char *s)
{
	int	i;
	int	len;

	i = 0;
	len = 0;
	if ((s[i] == '-' || s[i] == '+') && ft_strlen(s) > 1)
	{
		len++;
		i++;
	}
	while (s[i])
		if (ft_isdigit(s[i++]) == 0)
			return (0);
	if (ft_strlen(s) - len < 20)
		return (1);
	return (0);
}

int	mini_exit(char **args, int fd, t_shell *s, char **envp)
{
	int	has_second_arg;

	(void)envp;
	has_second_arg = (args[2] != NULL);
	ft_putstr_fd("exit\n", fd);
	if (args[1] == NULL)
		shell_exit(s, s->last_status, NULL);
	if (!str_isdigit(args[1]))
	{
		ft_fprintf(fd, "minishell: exit: %s: numeric argument required\n", fd);
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
