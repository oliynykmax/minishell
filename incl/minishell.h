#ifndef MINISHELL_H
# define MINISHELL_H

# include "../libft/libft.h"
# include <dirent.h>
# include <fcntl.h>
# include <signal.h>
# include <stdatomic.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <sys/ioctl.h>
# include <sys/stat.h>
# include <sys/types.h>
# include <sys/wait.h>
# include <term.h>
# include <termios.h>
# include <unistd.h>
# include <errno.h>
# include <readline/history.h>
# include <readline/readline.h>

# define ARENA_SIZE 10000 // Default size for new memory arenas.
# define BGRN	"\001\e[1;32m\002"
# define CRESET	"\001\e[0m\002"
# define STR_PROMPTSTART		"\001\e[1;96m\002\001\e[0m\002"
# define STR_PROMPTDELIM		"🐚> "

typedef volatile sig_atomic_t	t_signal;
typedef struct s_arena			t_arena;
typedef struct s_vec			t_vec;
typedef struct s_shell			t_shell;

struct s_shell
{
	char						*input;
	t_vec						*tokens;
	char						*cwd;
	t_arena						*arena;
	t_arena						*arenas[2];
	t_vec						*envp;
	size_t						prompt_count;
	unsigned char				last_status;
};

struct s_arena
{
	t_arena	*next;		// The next arena in the list.
	size_t	size;		// Size of used portion of arena.
	size_t	capacity;	// Total allocated size of arena.
	char	data[];		// Storage used for allocations.
};

struct s_vec
{
	t_shell	*shell;		// The shell that created the vector.
	size_t	size;		// The number of elements in the vector.
	size_t	capacity;	// The number of allocated elements.
	void	**data;		// Array of elements.
};

extern t_signal					g_signal;

void	handle_signals(int signum);
/*------------------shell------------------------------------------- */
void	shell_init(t_shell *s, char **envp);
void	shell_exit(t_shell *s, int exit_status, const char *message);
void	shell_new_prompt(t_shell *s);
void	*shell_malloc(t_shell *s, size_t size);
/*------------------memory arena----------------------------------- */
t_arena	*arena_new(t_shell *s, size_t capacity);
void	arena_reset(t_arena *arena);
void	arena_free(t_arena *arena);
/*-----------------vector-funcs------------------------------------- */
t_vec	*vector_new(t_shell *s, size_t capacity);
void	vector_push(t_vec *v, void *value);
void	vector_insert(t_vec *v, size_t index, void *value);
void	vector_delete(t_vec *v, size_t index);
/*------------------string-funcs-using-arena------------------------- */
char	*string_new(t_shell *s, const char *string);
char	*string_sub(t_shell *s, const char *string, size_t length);
char	*string_join(t_shell *s, const char *a, const char *b);
/*------------------tokenizer---------------------------------------- */
t_vec	*tokenize(t_shell *s, char *input);
/*------------------builtins----------------------------------------*/
int		pwd(char **argv, int fd);
int		env(char **envp, int fd);
void	mini_exit(char **args, int fd, t_shell *s);

/*------------------utils----------------------------------------- */
char	*get_working_dir(t_shell *s);
char	*get_prompt(t_shell *s);
/*----------------execution-------------------------------------- */
void	shell_execute(t_shell *s);

#endif
