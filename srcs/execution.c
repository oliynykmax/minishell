#include "../incl/minishell.h"

static int	unimplemented(char **argv, int fd, t_shell *s)
{
	(void) fd, (void) s;
	printf("Unimplemented builtin: %s\n", *argv++);
	while (*argv != NULL)
		printf("    Argument: %s\n", *argv++);
	return (EXIT_FAILURE);
}

static t_builtin	*get_builtin_by_name(char *name)
{
	if (ft_strcmp(name, "echo") == 0)
		return (mini_echo);
	if (ft_strcmp(name, "cd") == 0)
		return (unimplemented);
	if (ft_strcmp(name, "pwd") == 0)
		return (mini_pwd);
	if (ft_strcmp(name, "export") == 0)
		return (mini_export);
	if (ft_strcmp(name, "unset") == 0)
		return (unimplemented);
	if (ft_strcmp(name, "env") == 0)
		return (mini_env);
	if (ft_strcmp(name, "exit") == 0)
		return (mini_exit);
	return (NULL);
}

static void	error(const char *message)
{
	printf("minishell: %s\n", message);
}

static void	simple_command(t_shell *s, t_vec *command, t_vec *redirs)
{
	t_builtin	*builtin;
	char		*name;

	if (command->size == 0)
		return ;
	name = command->data[0];
	params_expand_vector(command);
	builtin = get_builtin_by_name(name);
	if (builtin != NULL)
		builtin((char **) command->data, 1, s);
	else
		subprocess_run(s, command, redirs);
}

void	shell_execute(t_shell *s)
{
	t_vec *const	command = vector_new(s, 0);
	t_vec *const	redirs = vector_new(s, 0);
	char **const	tokens = (char**) s->tokens->data;
	size_t			i;

	i = -1;
	while (tokens[++i] != NULL)
	{
		if (!ft_strcmp(tokens[i], "|"))
		{
			simple_command(s, command, redirs);
			command->size = 0;
			redirs->size = 0;
		}
		else if (!ft_strcmp(tokens[i], "<") || !ft_strcmp(tokens[i], ">"))
		{
			vector_push(redirs, tokens[i]);
			vector_push(redirs, tokens[++i]);
			if (tokens[i] == NULL)
				return (error("syntax error"));
		}
		else
			vector_push(command, tokens[i]);
	}
	simple_command(s, command, redirs);
}
