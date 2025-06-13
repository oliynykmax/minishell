#include "../incl/minishell.h"

static void	*run_command(t_shell *s, t_vec *command, t_vec *redirs, int fds[3])
{
	t_bn		*builtin;
	const char	*name = command->data[0];
	const pid_t	pid = fork();
	int			status;

	if (pid == -1)
		ft_fprintf(2, "minishell: fork: %s\n", strerror(errno));
	else if (pid > 0)
		loop_safe_close(&fds[0], 2);
	else
	{
		dup2(fds[0], STDIN_FILENO);
		dup2(fds[1], STDOUT_FILENO);
		loop_safe_close(&fds[0], 3);
		redirect(redirs);
		params_expand_vector(command);
		builtin = get_builtin_by_name((char *)name);
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

static void	handle_pipe_token(t_shell *s, t_vec *com, t_vec *redirs, int *fds)
{
	int	pipe_fd[2];

	pipe(pipe_fd);
	fds[1] = pipe_fd[1];
	vector_push(s->pids, run_command(s, com, redirs, fds));
	fds[0] = pipe_fd[0];
	fds[1] = -1;
	com->size = 0;
	redirs->size = 0;
}

static int	handle_redirect_token(char ***tokens, t_vec *redirs)
{
	vector_push(redirs, **tokens);
	(*tokens)++;
	vector_push(redirs, **tokens);
	return (**tokens == NULL);
}

void	shell_execute(t_shell *s, char **tokens)
{
	t_vec *const	command = vector_new(s, 0);
	t_vec *const	redirs = vector_new(s, 0);
	int				fds[3];

	fds[0] = STDIN_FILENO;
	fds[1] = -1;
	fds[2] = -1;
	s->pids = vector_new(s, 0);
	while (*tokens != NULL)
	{
		if (!ft_strcmp(*tokens, "|"))
			handle_pipe_token(s, command, redirs, fds);
		else if (**tokens == '<' || **tokens == '>')
		{
			if (handle_redirect_token(&tokens, redirs))
				return (error("syntax error"));
		}
		else
			vector_push(command, *tokens);
		tokens++;
	}
	fds[1] = STDOUT_FILENO;
	if (command->size != 0)
		vector_push(s->pids, run_command(s, command, redirs, fds));
	wait_for_all(s, s->pids);
}
