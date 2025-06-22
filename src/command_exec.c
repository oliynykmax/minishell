#include "minishell.h"

static void	handle_pipe_token(t_shell *s, t_vec *command, t_vec *redirs)
{
	int	pipe_fd[2];

	pipe(pipe_fd);
	s->fd_out = pipe_fd[1];
	s->fd_unused = pipe_fd[0];
	run_command(s, command, redirs);
	s->fd_in = pipe_fd[0];
	s->fd_out = STDOUT_FILENO;
	s->fd_unused = pipe_fd[1];
	command->size = 0;
	redirs->size = 0;
}

void	execute_command_pipeline(t_shell *s, char **tokens)
{
	t_vec *const	command = vector_new(s, 0);
	t_vec *const	redirs = vector_new(s, 0);

	s->fd_in = STDIN_FILENO;
	s->fd_out = STDOUT_FILENO;
	s->fd_unused = -1;
	s->pids = vector_new(s, 0);
	while (*tokens != NULL)
	{
		if (!ft_strcmp(*tokens, "|"))
			handle_pipe_token(s, command, redirs);
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
	s->fd_out = STDOUT_FILENO;
	run_command(s, command, redirs);
}
