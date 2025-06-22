#include "minishell.h"

void	init_subshell(t_shell *s)
{
	setup_signals(1);
	dup2(s->fd_in, STDIN_FILENO);
	dup2(s->fd_out, STDOUT_FILENO);
	safe_close(&s->fd_in);
	safe_close(&s->fd_out);
	safe_close(&s->fd_unused);
}

void	command_fork(t_shell *s, pid_t *pid)
{
	*pid = fork();
	if (*pid == -1)
	{
		ft_fprintf(2, "minishell: fork: %s\n", strerror(errno));
		return ;
	}
	else if (*pid != 0)
	{
		safe_close(&s->fd_in);
		safe_close(&s->fd_out);
		vector_push(s->pids, (void *)(intptr_t) * pid);
		return ;
	}
}

static void	handle_signal_termination(int status, int *last_status)
{
	if (WTERMSIG(status) == SIGINT)
		write(STDERR_FILENO, "\n", 1);
	else if (WTERMSIG(status) == SIGQUIT)
		ft_fprintf(STDERR_FILENO, "Quit (core dumped)\n");
	*last_status = 128 + WTERMSIG(status);
}

void	wait_for_all(t_shell *s, t_vec *pids)
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
