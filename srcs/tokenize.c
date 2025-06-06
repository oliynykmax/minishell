#include "libft.h"
#include "tokenize.h"

static t_token	*new_token(t_tokenizer *t)
{
	t_token	*new_tokens;

	if (t->num_tokens == t->max_tokens)
	{
		t->max_tokens = t->max_tokens * 2 + 64;
		new_tokens = ft_calloc(t->max_tokens, sizeof(t_token));
		if (new_tokens != NULL)
			ft_memcpy(new_tokens, t->tokens, t->num_tokens * sizeof(t_token));
		free(t->tokens);
		t->tokens = new_tokens;
	}
	if (t->tokens == NULL)
		return (NULL);
	return (&t->tokens[t->num_tokens++]);
}

static char	*tokenize_meta(t_token *t, char *input)
{
	t->data = NULL;
	if (input[0] == input[1])
	{
		if (*input == '<')
			t->type = TOKEN_HEREDOC;
		if (*input == '>')
			t->type = TOKEN_APPEND;
		if (*input == '&')
			t->type = TOKEN_AND;
		if (*input == '|')
			t->type = TOKEN_OR;
		return (input + 2);
	}
	if (*input == '|')
		t->type = TOKEN_PIPE;
	if (*input == '&')
		t->type = TOKEN_AMP;
	if (*input == '<')
		t->type = TOKEN_INPUT;
	if (*input == '>')
		t->type = TOKEN_OUTPUT;
	return (input + 1);
}

static char	*tokenize_word(t_token *t, char *input)
{
	char		end;

	end = '\0';
	t->type = TOKEN_WORD;
	if (*input == '\"')
		t->type = TOKEN_QUOTED;
	if (*input == '\'' || *input == '\"')
		end = *input++;
	t->data = input;
	while (*input != '\0' && *input != end && (end != '\0' || !is_meta(*input)))
		input++;
	t->length = input - t->data;
	return (input + (end != '\0'));
}

static void	null_terminate_words(t_token *token)
{
	while (token->type != TOKEN_END)
	{
		if (token->data != NULL)
			token->data[token->length] = '\0';
		token++;
	}
}

t_token	*tokenize(char *input)
{
	t_tokenizer	t;
	t_token		*token;

	ft_bzero(&t, sizeof(t));
	while (*input != '\0')
	{
		while (is_blank(*input))
			input++;
		if (*input != '\0')
		{
			token = new_token(&t);
			if (token == NULL)
				return (NULL);
			if (is_meta(*input))
				input = tokenize_meta(token, input);
			else
				input = tokenize_word(token, input);
		}
	}
	token = new_token(&t);
	if (token == NULL)
		return (NULL);
	token->type = TOKEN_END;
	null_terminate_words(t.tokens);
	return (t.tokens);
}
