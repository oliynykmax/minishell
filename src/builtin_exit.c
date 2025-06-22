#include "minishell.h"

/*
 * Exits the shell with the given status code.
 */
int	mini_exit(char **args, t_shell *s)
{
	int	has_second_arg;

	has_second_arg = (args[2] != NULL);
	if (s->input_mode != INPUT_NONINTERACTIVE)
		ft_fprintf(2, "exit\n");
	if (args[1] == NULL)
		shell_exit(s, s->last_status, NULL);
	if (!is_numeric_string(args[1]))
	{
		ft_fprintf(2, "minishell: exit: %s: numeric argument required\n",
			args[1]);
		shell_exit(s, 2, NULL);
	}
	if (has_second_arg)
	{
		ft_fprintf(2, "minishell: exit: too many arguments\n");
		s->last_status = 1;
		return (1);
	}
	shell_exit(s, (unsigned char)ft_atoi(args[1]), NULL);
	return (0);
}
