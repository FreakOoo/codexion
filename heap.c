/* ************************************************************************** */
/*                                                                            */
/*                                                I AM HIJACKING THE 42LOGO   */
/*   heap.c                                       THIS MESSAGE IS BROADCAST   */
/*                                                       BY FREAKO            */
/*   By: mchopin <mchopin@student.codam.nl>       WAWAWAWAWAWAWAWAWAWAWAWAW   */
/*                                                HEHE HERE'S YOUR NEW LOGO   */
/*   Created: 2026/10/17 18:58:34 by mchopin                42                */
/*   Updated: 2026/10/17 18:58:34 by mchopin      get shrinkified nerds >:)   */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

void	heap_push(t_stick *stick, t_ask_forstick ask)
{
	stick->heap[stick->size] = ask;
	sift_up(stick->heap, stick->size);
	stick->size++;
}

t_ask_forstick	heap_pop(t_stick *stick)
{
	t_ask_forstick	top;

	top = stick->heap[0];
	stick->size--;
	stick->heap[0] = stick->heap[stick->size];
	sift_down(stick->heap, stick->size, 0);
	return (top);
}

t_ask_forstick	*heap_peek(t_stick *stick)
{
	if (stick->size == 0)
		return (NULL);
	return (&stick->heap[0]);
}
