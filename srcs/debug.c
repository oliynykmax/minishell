#include "../incl/minishell.h"

void debug_mode(t_shell *s, char *input, char **envp)
{
	char *av_input = string_new(s, input);
	s->tokens = tokenize(s, av_input);
	printf("Our shell:\n");
	shell_execute(s);
	char *bash_args[] = {"bash", "-c", av_input, NULL};
	printf("Bash:\n");
	execve("/bin/bash", bash_args, envp);
}
