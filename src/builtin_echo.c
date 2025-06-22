#include "minishell.h"

/*
 * echo command
 * always returns 0
 * writes to the file descriptor passed as argument
 */
int	mini_echo(char **argv, t_shell *s)
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
		ft_fprintf(STDOUT_FILENO, "%s", argv[i]);
		if (argv[i + 1])
			ft_fprintf(STDOUT_FILENO, " ");
		i++;
	}
	if (if_new_line)
		ft_fprintf(STDOUT_FILENO, "\n");
	(void)s;
	return (0);
}
