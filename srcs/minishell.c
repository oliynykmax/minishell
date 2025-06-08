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

static void	free_tokens(t_token *tokens)
{
	if (tokens == NULL)
		return ;
	free(tokens);
}

static void	init_shell(t_shell *shell)
{
	shell->input = NULL;
	shell->tokens = NULL;
	shell->cwd = NULL;
	shell->sa.sa_handler = handle_signals;
	sigemptyset(&shell->sa.sa_mask);
	shell->sa.sa_flags = SA_RESTART;
	if (sigaction(SIGINT, &shell->sa, NULL) == -1)
	{
		perror("sigaction for SIGINT failed");
		exit(EXIT_FAILURE);
	}
	signal(SIGQUIT, SIG_IGN);
}

static void	cleanup_cycle_resources(t_shell *shell)
{
	if (shell->tokens)
	{
		free_tokens(shell->tokens);
		shell->tokens = NULL;
	}
	if (shell->input)
	{
		free(shell->input);
		shell->input = NULL;
	}
}

static void cleanup_shell(t_shell *shell)
{
	if (shell->cwd)
	{
		free(shell->cwd);
		shell->cwd = NULL;
	}
	cleanup_cycle_resources(shell);
}


static t_sstatus	process_command_line(t_shell *shell, char *input)
{
	t_sstatus	status;

	status = SHELL_CONTINUE;
	shell->input = input;
	add_history(shell->input);
	shell->tokens = tokenize(shell->input);
	if (shell->tokens == NULL)
	{
		fprintf(stderr, "shell: tokenization failed\n");
		cleanup_cycle_resources(shell);
		return (SHELL_CONTINUE);
	}
	print_tokens(shell->tokens);
	if (shell->tokens[0].data && ft_strcmp(shell->tokens[0].data, "exit") == 0)
		status = SHELL_EXIT;
	/* 	else
		{
			// TODO:
		} */
	cleanup_cycle_resources(shell);
	return (status);
}

static void	create_prompt(t_shell *shell)
{
	if (shell->cwd)
		free(shell->cwd);
	shell->cwd = malloc(PATH_MAX);
	if (shell->cwd == NULL)
	{
		perror("malloc failed");
		exit(EXIT_FAILURE);
	}
	if (getcwd(shell->cwd, PATH_MAX) == NULL)
	{
		perror("getcwd failed");
		free(shell->cwd);
		shell->cwd = NULL;
	}
	else
		ft_strlcat(shell->cwd, "\n🐚> ", PATH_MAX);
}

static void	shell_loop(t_shell *shell)
{
	char	*current_input;

	while (1)
	{
		create_prompt(shell);
		g_signal = 0;
		current_input = readline(shell->cwd);
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
		if (process_command_line(shell, current_input) == SHELL_EXIT)
		{
			free(current_input);
			break ;
		}
	}
}

int	main(void)
{
	t_shell	shell;

	init_shell(&shell);
	shell_loop(&shell);
	cleanup_shell(&shell);
	rl_clear_history();
	return (0);
}
