#include "../incl/minishell.h"

// Find the first unquoted occurrence of a wildcard character in a string, or
// NULL if none was found.

static char	*find_unquoted_wildcard(char *str)
{
	char	quote;

	quote = 0;
	while (*str != '\0')
	{
		if (!quote)
		{
			if (*str == '\'' || *str == '\"')
				quote = *str;
			else if (*str == '*')
				return (str);
		}
		else if (*str == quote)
			quote = 0;
		str++;
	}
	return (NULL);
}

// Check if a filename matches a glob pattern. The pattern can either be a
// literal filename, or it can contain a wildcard.

static bool	match_pattern(char *filename, char *pattern)
{
	char	*tail;
	char	*wildcard;

	wildcard = find_unquoted_wildcard(pattern);
	if (!wildcard++)
		return (!ft_strcmp(filename, pattern));
	tail = filename + ft_strlen(filename) - ft_strlen(wildcard);
	return (!ft_strncmp(filename, pattern, wildcard - (pattern + 1))
		&& filename <= tail && !ft_strcmp(tail, wildcard));
}

// Create a list of all files in the current working directory, not including
// dotfiles, or the special entries "." and "..".

static t_vec	*list_files(t_shell *s)
{
	struct dirent	*entry;
	t_vec			*files;

	files = vector_new(s, 0);
	s->dirent = opendir(".");
	if (!s->dirent)
		ft_fprintf(2, "minishell: opendir: %s\n", strerror(errno));
	else
	{
		entry = readdir(s->dirent);
		while (entry != NULL)
		{
			if (entry->d_name[0] != '.')
				vector_push(files, string_new(s, entry->d_name));
			entry = readdir(s->dirent);
		}
		closedir(s->dirent);
		s->dirent = NULL;
	}
	if (files->size > 0)
		sort_strings((char **) files->data);
	return (files);
}

// Apply filename expansion on all tokens in a vector. Each token is replaced
// by any filenames that match that token in the current working directory.
// If no files match, the token is left in place untouched.

void	filename_expand(t_vec *tokens)
{
	t_vec	*files;
	char	*pattern;
	size_t	i;
	size_t	j;
	size_t	k;

	i = 0;
	files = list_files(tokens->shell);
	while (i < tokens->size)
	{
		j = 0;
		k = i;
		pattern = tokens->data[i];
		vector_delete(tokens, i);
		while (j < files->size)
		{
			if (match_pattern(files->data[j], pattern))
				vector_insert(tokens, k++, files->data[j]);
			j++;
		}
		if (i++ == k)
			vector_insert(tokens, k, pattern);
	}
}

// Apply tilde expansion to all words in a vector. Replaces the `~` character
// with the value of $HOME if it appears at the beginning of a word, and either
// appears on its own, or is immediately followed by a slash. If $HOME is unset,
// the path /home/$USER is used instead. If $USER is also unset, no tilde
// expansion is done.

void	tilde_expand_vector(t_vec *t)
{
	char	*home;
	char	*user;
	char	**str;

	if (t->size == 0)
		return ;
	home = get_env_variable(t->shell, "HOME");
	user = get_env_variable(t->shell, "USER");
	if (*home == '\0')
	{
		if (*user != '\0')
			home = string_join(t->shell, "/home/", user);
		else
			return ;
	}
	str = (char **) t->data;
	while (*str != NULL)
	{
		if (str[0][0] == '~' && (str[0][1] == '/' || str[0][1] == '\0'))
			*str = string_join(t->shell, home, *str + 1);
		str++;
	}
}
