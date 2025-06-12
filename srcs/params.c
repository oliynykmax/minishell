#include "../incl/minishell.h"

char	*get_env_variable(t_shell *s, char *key)
{
	char	**envp;
	char	*equals;

	envp = (char **) s->envp->data;
	while (*envp != NULL)
	{
		equals = ft_strchr(*envp, '=');
		if (equals != NULL && ft_strncmp(*envp, key, equals - *envp) == 0)
			return (equals + 1);
		envp++;
	}
	return ("");
}

static char	*params_replace(t_shell *s, char **out, char *in)
{
	char	*key;
	char	*start;

	start = in;
	if (*in == '?')
	{
		*out = string_join(s, *out, string_itoa(s, s->last_status));
		return (in + 1);
	}
	if (ft_isalpha(*in) || *in == '_')
		in++;
	while (ft_isalpha(*in) || ft_isdigit(*in) || *in == '_')
		in++;
	if (in == start)
		*out = string_join(s, *out, "$");
	else
	{
		key = string_sub(s, start, in - start);
		*out = string_join(s, *out, get_env_variable(s, key));
	}
	return (in);
}

char	*params_expand_string(t_shell *s, char *in)
{
	char	*out;
	char	*start;
	char	quote;

	quote = 0;
	out = "";
	start = in;
	while (*in != '\0')
	{
		if (!quote && (*in == '\'' || *in == '\"'))
			quote = *in;
		else if (quote && *in == quote)
			quote = 0;
		else if (quote != '\'' && *in == '$')
		{
			out = string_join(s, out, string_sub(s, start, in - start));
			in = params_replace(s, &out, in + 1) - 1;
			start = in + 1;
		}
		in++;
	}
	out = string_join(s, out, string_sub(s, start, in - start));
	return (out);
}

void	params_expand_vector(t_vec *tokens)
{
	size_t	i;

	i = -1;
	while (++i < tokens->size)
		tokens->data[i] = params_expand_string(tokens->shell, tokens->data[i]);
}
