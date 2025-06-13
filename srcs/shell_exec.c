#include "../incl/minishell.h"

static void	run_child_process(t_shell *s, t_vec *command, t_vec *redirs,
	int fds[3])
{
	t_bn		*builtin;
	int			status;

	setup_child_signals();
	dup2(fds[0], STDIN_FILENO);
	dup2(fds[1], STDOUT_FILENO);
	loop_safe_close(&fds[0], 3);
	redirect(redirs);
	params_expand_vector(command);
	builtin = get_builtin_by_name((char *)command->data[0]);
	if (builtin != NULL)
	{
		status = builtin((char **) command->data, 1, s);
		shell_exit(s, status, NULL);
	}
	subprocess_run(s, command, redirs);
}

void	*run_command(t_shell *s, t_vec *command, t_vec *redirs, int fds[3])
{
	const pid_t	pid = fork();

	if (pid == -1)
		ft_fprintf(2, "minishell: fork: %s\n", strerror(errno));
	else if (pid > 0)
		loop_safe_close(&fds[0], 2);
	else
		run_child_process(s, command, redirs, fds);
	return ((void *)(intptr_t) pid);
}

static void	handle_signal_termination(int status, int *last_status)
{
	if (WTERMSIG(status) == SIGINT)
		write(STDOUT_FILENO, "\n", 1);
	else if (WTERMSIG(status) == SIGQUIT)
		write(STDOUT_FILENO, "Quit\n", 5);
	*last_status = 128 + WTERMSIG(status);
}

static void	wait_for_all(t_shell *s, t_vec *pids)
{
	size_t	i;
	int		status;
	int		last_status;

	i = 0;
	last_status = 0;
	signal(SIGINT, SIG_IGN);
	signal(SIGQUIT, SIG_IGN);
	while (i < pids->size)
	{
		waitpid((pid_t)(intptr_t) pids->data[i++], &status, 0);
		if (WIFEXITED(status))
			last_status = WEXITSTATUS(status);
		else if (WIFSIGNALED(status))
			handle_signal_termination(status, &last_status);
	}
	setup_parent_signals();
	s->last_status = last_status;
}

void	shell_execute(t_shell *s, char **tokens)
{
	execute_command_pipeline(s, tokens);
	wait_for_all(s, s->pids);
}
