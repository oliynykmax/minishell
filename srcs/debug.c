#include "../incl/minishell.h"
/*
 * debug mode for minishell
 * needs to be removed later, idk.
 */
void	debug_mode(t_shell *s, char *input, char **envp)
{
	const char		*av_input = string_new(s, input);
	const char		*bash_args[] = {"bash", "-c", (char *)av_input, NULL};

	s->tokens = tokenize(s, (char *)av_input);
	printf("Our shell:\n");
	shell_execute(s, (char **) s->tokens->data);
	printf("Bash:\n");
	execve("/bin/bash", (char **)bash_args, envp);
}
