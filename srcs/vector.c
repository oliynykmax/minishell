#include "../incl/minishell.h"

// Create a new empty vector, optionally with an initial capacity.

t_vec	*vector_new(t_shell *s, size_t capacity)
{
	t_vec *const	v = shell_malloc(s, sizeof(t_vec));

	v->shell = s;
	if (capacity > 0)
	{
		v->capacity = capacity;
		v->data = shell_malloc(s, (capacity + 1) * sizeof(*v->data));
	}
	return (v);
}

// Add an element to the end of a vector. If there's no more space for elements,
// the vector is reallocated.

void	vector_push(t_vec *v, void *value)
{
	void	**new_data;

	if (v->size >= v->capacity)
	{
		v->capacity = v->capacity * 2 + 32;
		new_data = shell_malloc(v->shell, (v->capacity + 1) * sizeof(*v->data));
		v->data = ft_memcpy(new_data, v->data, v->size * sizeof(*v->data));
	}
	v->data[v->size++] = value;
	v->data[v->size] = NULL;
}

// Insert a value into a vector, placing it at the given index, and shifting any
// subsequent elements to make room. The index must be less than or equal to the
// current size of the vector.

void	vector_insert(t_vec *v, size_t index, void *value)
{
	const size_t	bytes_moved = (v->size - index + 1) * sizeof(*v->data);

	if (index > v->size)
		return ;
	vector_push(v, NULL);
	ft_memmove(v->data + index + 1, v->data + index, bytes_moved);
	v->data[index] = value;
}

// Delete the element at the given index from a vector, shifting any subsequent
// elements into the gap left by the deleted element. The index must be less
// than the current size of the vector. If the vector is empty, nothing is done.

void	vector_delete(t_vec *v, size_t index)
{
	const size_t	bytes_moved = (v->size - index) * sizeof(*v->data);

	if (index >= v->size)
		return ;
	ft_memmove(v->data + index, v->data + index + 1, bytes_moved);
	v->size--;
}
