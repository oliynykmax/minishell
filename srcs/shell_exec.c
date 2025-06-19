#include "../incl/minishell.h"

static void	init_subshell(t_shell *s)
{
	setup_signals(1);
	dup2(s->fd_in, STDIN_FILENO);
	dup2(s->fd_out, STDOUT_FILENO);
	safe_close(&s->fd_in);
	safe_close(&s->fd_out);
	safe_close(&s->fd_unused);
}

void	run_builtin(t_shell *s, t_vec *command, t_vec *redirs)
{
	t_bn *const	builtin = get_builtin_by_name(command->data[0]);
	const bool	pipelined = s->fd_in != 0 || s->fd_out != 1;

	if (pipelined)
	{
		ignore_sigpipe();
		init_subshell(s);
		if (redirect(s, redirs))
			s->last_status = builtin((char **) command->data, STDOUT_FILENO, s);
		shell_exit(s, s->last_status, NULL);
	}
	s->fd_saved_in = dup(STDIN_FILENO);
	s->fd_saved_out = dup(STDOUT_FILENO);
	if (redirect(s, redirs))
		s->last_status = builtin((char **) command->data, STDOUT_FILENO, s);
	dup2(s->fd_saved_in, STDIN_FILENO);
	dup2(s->fd_saved_out, STDOUT_FILENO);
	safe_close(&s->fd_saved_in);
	safe_close(&s->fd_saved_out);
}

void	run_program(t_shell *s, t_vec *command, t_vec *redirs)
{
	init_subshell(s);
	if (!redirect(s, redirs))
		shell_exit(s, 1, NULL);
	subprocess_run(s, command);
}

void	run_command(t_shell *s, t_vec *command, t_vec *redirs)
{
	t_bn		*builtin;
	pid_t		pid;

	params_expand_vector(command);
	params_expand_vector(redirs);
	if (command->size == 0)
		return ;
	builtin = get_builtin_by_name(command->data[0]);
	pid = -1;
	if (builtin == NULL || s->fd_in != 0 || s->fd_out != 1)
	{
		pid = fork();
		if (pid == -1)
		{
			ft_fprintf(2, "minishell: fork: %s\n", strerror(errno));
			return ;
		}
		else if (pid != 0)
		{
			safe_close(&s->fd_in);
			safe_close(&s->fd_out);
			vector_push(s->pids, (void *)(intptr_t)pid);
			return ;
		}
	}
	if (builtin != NULL)
		run_builtin(s, command, redirs);
	else
		run_program(s, command, redirs);
}

static void	handle_signal_termination(int status, int *last_status)
{
	if (WTERMSIG(status) == SIGINT)
		write(STDERR_FILENO, "\n", 1);
	else if (WTERMSIG(status) == SIGQUIT)
		ft_fprintf(STDERR_FILENO, "Quit (core dumped)\n");
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
		s->last_status = last_status;
	}
	setup_signals(0);
}

void	shell_execute(t_shell *s, char **tokens)
{
	execute_command_pipeline(s, tokens);
	wait_for_all(s, s->pids);
	clear_temp_files(s);
}
