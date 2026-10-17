/* ************************************************************************** */
/*                                                                            */
/*                                                I AM HIJACKING THE 42LOGO   */
/*   heap_utils.c                                 THIS MESSAGE IS BROADCAST   */
/*                                                       BY FREAKO            */
/*   By: mchopin <mchopin@student.codam.nl>       WAWAWAWAWAWAWAWAWAWAWAWAW   */
/*                                                HEHE HERE'S YOUR NEW LOGO   */
/*   Created: 2026/10/17 18:58:35 by mchopin                42                */
/*   Updated: 2026/10/17 18:58:35 by mchopin      get shrinkified nerds >:)   */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

int	heap_less(t_ask_forstick *a, t_ask_forstick *b)
{
	if (a->key != b->key)
		return (a->key < b->key);
	return (a->permission < b->permission);
}

static void	swap_asks(t_ask_forstick *a, t_ask_forstick *b)
{
	t_ask_forstick	tmp;

	tmp = *a;
	*a = *b;
	*b = tmp;
}

void	sift_up(t_ask_forstick *heap, int i)
{
	int	parent;

	while (i > 0)
	{
		parent = (i - 1) / 2;
		if (!heap_less(&heap[i], &heap[parent]))
			break ;
		swap_asks(&heap[i], &heap[parent]);
		i = parent;
	}
}

void	sift_down(t_ask_forstick *heap, int size, int i)
{
	int	child;

	while (2 * i + 1 < size)
	{
		child = 2 * i + 1;
		if (child + 1 < size && heap_less(&heap[child + 1], &heap[child]))
			child++;
		if (!heap_less(&heap[child], &heap[i]))
			break ;
		swap_asks(&heap[child], &heap[i]);
		i = child;
	}
}
