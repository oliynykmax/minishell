#include "../incl/minishell.h"

// Create a new arena with a given capacity. If memory allocation fails, cleans
// up all resources and exits with an error message.

t_arena	*arena_new(t_shell *s, size_t capacity)
{
	t_arena *const	arena = ft_calloc(1, sizeof(t_arena) + capacity);

	if (arena == NULL)
		shell_exit(s, EXIT_FAILURE, "out of memory");
	arena->capacity = capacity;
	ft_memset(arena->data, 'A', arena->capacity);
	printf("arena_new(..., %zu)\n", capacity);
	return (arena);
}

// Reset an arena, making all of its storage available for reuse. The arena's
// storage is cleared to the character 'A' to make it easier to catch memory
// errors.

void	arena_reset(t_arena *arena)
{
	while (arena != NULL)
	{
		arena->size = 0;
		ft_memset(arena->data, 'A', arena->capacity);
		arena = arena->next;
	}
}

// Free all memory for an arena, and all subsequent arenas in the linked list.
// The arena is no longer usable after this.

void	arena_free(t_arena *arena)
{
	t_arena	*next;

	while (arena != NULL)
	{
		next = arena->next;
		free(arena);
		arena = next;
	}
}

// Allocate memory out of the current memory arena. Always returns a valid
// memory allocation; if an error occurs, all resources are cleaned up and the
// program exits with an error message. The returned memory is cleared to zero,
// and doesn't need to be explicitly freed; it is automatically recycled at the
// beginning of the next prompt.

void	*shell_malloc(t_shell *s, size_t size)
{
	t_arena	*arena;
	t_arena	*new_arena;
	void	*result;
	size_t	capacity;

	arena = s->arena;
	while (arena->next != NULL && arena->size + size > arena->capacity)
		arena = arena->next;
	if (arena->size + size > arena->capacity)
	{
		capacity = size;
		if (size < ARENA_SIZE)
			capacity = ARENA_SIZE;
		new_arena = arena_new(s, capacity);
		arena->next = new_arena;
		arena = new_arena;
	}
	result = &arena->data[arena->size];
	arena->size = (arena->size + size + ARENA_ALIGN - 1) & -ARENA_ALIGN;
	ft_bzero(result, size);
	return (result);
}
