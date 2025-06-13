#include "../incl/minishell.h"

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

void	execute_command_pipeline(t_shell *s, char **tokens)
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
}
