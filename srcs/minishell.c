#include "../incl/minishell.h"
#include "../incl/tokenize.h"
#include <linux/limits.h>
#include <stddef.h>

volatile sig_atomic_t	g_signal = 0;

static void	print_tokens(t_token *token)
{
	static const char *const	names[] = {"|", "&", "||", "&&", "<", "<<", ">",
		">>", "(", ")", "word", "word"};

	while (token->type != TOKEN_END)
	{
		if (token->data == NULL)
			printf("operator(\"%s\") ", names[token->type]);
		else
			printf("%s(\"%s\") ", names[token->type], token->data);
		token++;
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
	free(s->cwd);
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
	t_vector *const	old_envp = s->envp;
	size_t			i;

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
	s->tokens = tokenize(s->input);
	if (s->tokens == NULL)
	{
		fprintf(stderr, "shell: tokenization failed\n");
		return (SHELL_CONTINUE);
	}
	print_tokens(s->tokens);
	if (s->tokens[0].data && ft_strcmp(s->tokens[0].data, "exit") == 0)
		status = SHELL_EXIT;
	/* 	else
		{
			// TODO:
		} */
	return (status);
}

static void	create_prompt(t_shell *s)
{
	if (s->cwd)
		free(s->cwd);
	s->cwd = malloc(PATH_MAX);
	if (s->cwd == NULL)
	{
		perror("malloc failed");
		exit(EXIT_FAILURE);
	}
	if (getcwd(s->cwd, PATH_MAX) == NULL)
	{
		perror("getcwd failed");
		free(s->cwd);
		s->cwd = NULL;
	}
	else
		ft_strlcat(s->cwd, "\n🐚> ", PATH_MAX);
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

	(void) argc;
	(void) argv;
	shell_init(&shell, envp);
	shell_loop(&shell);
	rl_clear_history();
	shell_exit(&shell, 0, NULL);
}
