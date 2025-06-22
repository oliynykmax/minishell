#include "minishell.h"

void	run_builtin(t_shell *s, t_vec *command, t_vec *redirs)
{
	const bool	pipelined = s->fd_in != 0 || s->fd_out != 1;
	t_bn		*builtin;

	if (command->size == 0)
		builtin = get_builtin_by_name(NULL);
	else
		builtin = get_builtin_by_name(command->data[0]);
	if (pipelined)
	{
		ignore_sigpipe();
		init_subshell(s);
		if (redirect(s, redirs))
			s->last_status = builtin((char **) command->data, s);
		safe_close(&s->fd_unused);
		shell_exit(s, s->last_status, NULL);
	}
	s->fd_saved_in = dup(STDIN_FILENO);
	s->fd_saved_out = dup(STDOUT_FILENO);
	if (redirect(s, redirs))
		s->last_status = builtin((char **) command->data, s);
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
		builtin = get_builtin_by_name(NULL);
	else
		builtin = get_builtin_by_name(command->data[0]);
	pid = -1;
	if (builtin == NULL || s->fd_in != 0 || s->fd_out != 1)
	{
		command_fork(s, &pid);
		if (pid == -1 || pid != 0)
			return ;
	}
	if (builtin != NULL)
		run_builtin(s, command, redirs);
	else
		run_program(s, command, redirs);
}

void	shell_execute(t_shell *s, char **tokens)
{
	execute_command_pipeline(s, tokens);
	wait_for_all(s, s->pids);
	clear_temp_files(s);
}
