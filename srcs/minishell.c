#include "../incl/minishell.h"
#include "../incl/tokenize.h"

volatile sig_atomic_t	g_signal = 0;

static void	print_tokens(t_token *token)
{
	static const char *const	names[] = {
		"|", "&", "||", "&&", "<", "<<", ">", ">>", "(", ")", "word", "word"
	};

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

/*int	main(void)
{
	char	*input;
	t_token	*tokens;

	while (1)
	{
		input = readline("🐚> ");
		tokens = tokenize(input);
		print_tokens(tokens);
		if (ft_strcmp(tokens[0].data, "exit") == 0)
			return (0);
		free(tokens);
		free(input);
	}
	return (0);
	}*/

int	main(void)
{
	char	*input;
	t_token	*tokens;
    struct sigaction sa;

    sa.sa_handler = handle_signals;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = SA_RESTART;
    if (sigaction(SIGINT, &sa, NULL) == -1) {
        perror("sigaction for SIGINT failed");
        exit(EXIT_FAILURE);
    }
    signal(SIGQUIT, SIG_IGN);
	while (1)
	{
        g_signal = 0;
		input = readline("🐚> ");
        if (g_signal) {
            if (input) {
                free(input);
                input = NULL;
            }
            continue;
        }
		if (input == NULL)
		{
			printf("exit\n");
			break;
		}
        if (strlen(input) > 0)
            add_history(input);
        else
        {
            free(input);
            continue;
        }
		tokens = tokenize(input);
        if (tokens == NULL)
        {
            fprintf(stderr, "shell: tokenization failed\n");
            free(input);
            continue;
        }
		print_tokens(tokens);
		if (ft_strcmp(tokens[0].data, "exit") == 0)
		{
            free(tokens);
			free(input);
			break;
		}
		free(tokens);
		free(input);
	}
    rl_clear_history();
	return (0);
}
