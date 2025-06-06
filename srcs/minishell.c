/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abostrom <abostrom@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/06 16:09:01 by abostrom          #+#    #+#             */
/*   Updated: 2025/06/06 16:09:35 by abostrom         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>

#include "minishell.h"
#include "tokenize.h"

static void	print_tokens(t_token *token)
{
	static const char *const	names[] = {
		"|", "&", "||", "&&", "<", "<<", ">", ">>", "word", "word"
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

int	main(void)
{
	char	*input;
	t_token	*tokens;

	while (1)
	{
		input = readline("🐚> ");
		tokens = tokenize(input);
		print_tokens(tokens);
		if (tokens[0].type == TOKEN_WORD && strcmp(tokens[0].data, "exit") == 0)
			return (0);
		free(tokens);
		free(input);
	}
	return (0);
}
