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
		return (mini_unset);
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

static void	safe_close(int *fd)
{
	if (*fd != -1 && *fd != STDIN_FILENO && *fd != STDOUT_FILENO)
	{
		close(*fd);
		*fd = -1;
	}
}

static void	*run_command(t_shell *s, t_vec *command, t_vec *redirs, int fds[3])
{
	t_builtin	*builtin;
	char		*name;
	const pid_t	pid = fork();
	int			status;

	if (pid == -1)
		ft_fprintf(2, "minishell: fork: %s\n", strerror(errno));
	else if (pid > 0)
	{
		safe_close(&fds[0]);
		safe_close(&fds[1]);
	}
	else
	{
		dup2(fds[0], STDIN_FILENO);
		dup2(fds[1], STDOUT_FILENO);
		safe_close(&fds[0]);
		safe_close(&fds[1]);
		safe_close(&fds[2]);
		redirect(redirs);
		name = command->data[0];
		params_expand_vector(command);
		builtin = get_builtin_by_name(name);
		if (builtin != NULL)
		{
			status = builtin((char **) command->data, 1, s);
			shell_exit(s, status, NULL);
		}
		subprocess_run(s, command, redirs);
	}
	return ((void *)(intptr_t) pid);
}

static void	wait_for_all(t_shell *s, t_vec *pids)
{
	size_t	i;
	int		status;

	i = 0;
	status = 0;
	while (i < pids->size)
		waitpid((pid_t)(intptr_t) pids->data[i++], &status, 0);
	s->last_status = status;
}

void	shell_execute(t_shell *s, char **tokens)
{
	t_vec *const	command = vector_new(s, 0);
	t_vec *const	redirs = vector_new(s, 0);
	t_vec *const	pids = vector_new(s, 0);
	int				pipe_fd[2];
	int				fds[3];

	fds[0] = STDIN_FILENO;
	fds[1] = -1;
	fds[2] = -1;
	while (*tokens != NULL)
	{
		if (!ft_strcmp(*tokens, "|"))
		{
			pipe(pipe_fd);
			fds[1] = pipe_fd[1];
			fds[2] = pipe_fd[0];
			vector_push(pids, run_command(s, command, redirs, fds));
			fds[0] = pipe_fd[0];
			command->size = 0;
			redirs->size = 0;
		}
		else if (**tokens == '<' || **tokens == '>')
		{
			vector_push(redirs, *tokens++);
			vector_push(redirs, *tokens);
			if (*tokens == NULL)
				return (error("syntax error"));
		}
		else
			vector_push(command, *tokens);
		tokens++;
	}
	fds[1] = STDOUT_FILENO;
	if (command->size != 0)
		vector_push(pids, run_command(s, command, redirs, fds));
	wait_for_all(s, pids);
}
