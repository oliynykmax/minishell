#ifndef TOKENIZE_H
# define TOKENIZE_H

# include <stddef.h>
# include "../libft/libft.h"

typedef enum e_token_type
{
	TOKEN_PIPE,		/* | operator */
	TOKEN_AMP,		/* & operator */
	TOKEN_OR,		/* || operator */
	TOKEN_AND,		/* && operator */
	TOKEN_INPUT,	/* < operator */
	TOKEN_HEREDOC,	/* << operator */
	TOKEN_OUTPUT,	/* > operator */
	TOKEN_APPEND,	/* >> operator */
	TOKEN_LPAREN,	/* ( operator */
	TOKEN_RPAREN,	/* ) operator */
	TOKEN_WORD,		/* Non-double-quoted words */
	TOKEN_QUOTED,	/* Double-quoted words */
	TOKEN_END,		/* End of input */
}	t_token_type;

typedef struct s_token
{
	t_token_type	type;	/* The type of the token, one of TOKEN_xxx */
	char			*data;	/* String data of the token (only for words) */
	size_t			length;	/* Length of the token (only for words) */
}	t_token;

typedef struct s_tokenizer
{
	char	*input;
	t_token	*tokens;
	size_t	num_tokens;
	size_t	max_tokens;
}	t_tokenizer;

t_token	*tokenize(char *input);
int		is_blank(char c);
int		is_meta(char c);

#endif
