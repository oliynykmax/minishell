#include "../incl/minishell.h"
#include <errno.h>
#include <linux/limits.h>
#include <stddef.h>

volatile sig_atomic_t	g_signal = 0;

static void	print_tokens(t_vec *tokens)
{
	size_t	i;

	i = 0;
	while (i < tokens->size)
	{
		printf("[%s] ", (char *)tokens->data[i]);
		i++;
	}
	printf("\n");
}

void	shell_init(t_shell *s, char **envp)
{
	ft_bzero(s, sizeof(t_shell));
	s->arenas[0] = arena_new(s, ARENA_SIZE);
	s->arenas[1] = arena_new(s, ARENA_SIZE);
	shell_new_prompt(s);
	s->envp = vector_new(s, 0);
	while (*envp != NULL)
		vector_push(s->envp, string_new(s, *envp++));
	s->sa.sa_handler = handle_signals;
	sigemptyset(&s->sa.sa_mask);
	s->sa.sa_flags = SA_RESTART;
	if (sigaction(SIGINT, &s->sa, NULL) == -1)
	{
		perror("sigaction for SIGINT failed");
		exit(EXIT_FAILURE);
	}
	signal(SIGQUIT, SIG_IGN);
}

// Clean up all resources and exit the shell, with an optional error message.

void	shell_exit(t_shell *s, int exit_status, const char *message)
{
	free(s->input);
	arena_free(s->arenas[0]);
	arena_free(s->arenas[1]);
	if (message != NULL)
		printf("error: %s\n", message);
	exit(exit_status);
}

// Begin a new prompt, starting over with a new, empty memory arena. Environment
// variables are copied into the new arena so that they are preserved across
// prompts.

void	shell_new_prompt(t_shell *s)
{
	size_t			i;
	t_vec *const	old_envp = s->envp;

	s->prompt_count++;
	s->arena = s->arenas[s->prompt_count % 2];
	arena_reset(s->arena);
	if (old_envp != NULL)
	{
		s->envp = vector_new(s, old_envp->capacity);
		i = 0;
		while (i < old_envp->size)
			vector_push(s->envp, string_new(s, old_envp->data[i++]));
	}
}

static t_sstatus	process_command_line(t_shell *s, char *input)
{
	t_sstatus	status;

	status = SHELL_CONTINUE;
	s->input = input;
	add_history(s->input);
	s->tokens = tokenize(s, input);
	print_tokens(s->tokens);
	if (ft_strcmp(s->tokens->data[0], "exit") == 0)
		status = SHELL_EXIT;
	/* 	else
		{
			// TODO:
		} */
	return (status);
}

static void	create_prompt(t_shell *s)
{
	char			*pwd;
	char			*tmp;
	static size_t	bufsize;

	pwd = NULL;
	bufsize = 128;
	while (1)
	{
		pwd = shell_malloc(s, bufsize);
		if (getcwd(pwd, bufsize))
			break ;
		if (errno == ERANGE)
			bufsize *= 2;
	}
	tmp = string_join(s, "\n", pwd);
	s->cwd = string_join(s, tmp, "\n🐚> ");
}

static void	shell_loop(t_shell *s)
{
	char	*current_input;

	while (1)
	{
		shell_new_prompt(s);
		create_prompt(s);
		g_signal = 0;
		current_input = readline(s->cwd);
		if (current_input == NULL)
		{
			printf("exit\n");
			break ;
		}
		if (g_signal || current_input[0] == '\0')
		{
			free(current_input);
			continue ;
		}
		if (process_command_line(s, current_input) == SHELL_EXIT)
		{
			free(current_input);
			break ;
		}
	}
}

int	main(int argc, char **argv, char **envp)
{
	t_shell	shell;

	(void)argc;
	(void)argv;
	shell_init(&shell, envp);
	shell_loop(&shell);
	rl_clear_history();
	shell_exit(&shell, 0, NULL);
}
